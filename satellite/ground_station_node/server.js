const http = require("http");
const fs = require("fs");
const path = require("path");

const staticRoot = path.join(__dirname, "..", "ground_station", "static");
const port = Number(process.env.ASTERIA_PORT || 8765);
const host = process.env.ASTERIA_HOST || "127.0.0.1";

const validCommands = new Set([
    "safe",
    "nominal",
    "science",
    "downlink",
    "stop_payload",
    "reset",
    "step",
]);

function newState() {
    return {
        name: "Asteria-1",
        callsign: "AST1",
        software_version: "0.2.0-node",
        tick: 0,
        mode: "BOOT",
        battery_percent: 78,
        bus_temp_c: 18,
        solar_input_percent: 66,
        attitude_lock: false,
        payload_active: false,
        downlink_active: false,
        rejected_commands: 0,
        frames_sent: 0,
        radiation_counts: 100,
        camera_temp_c: 12,
        image_ready: false,
        storage: {
            capacity_packets: 1024,
            used_packets: 0,
            dropped_packets: 0,
        },
        events: [],
    };
}

let state = newState();
event("INFO", "boot", "node ground station initialized");

function event(severity, source, message) {
    state.events.push({ tick: state.tick, severity, source, message });
    if (state.events.length > 128) {
        state.events = state.events.slice(-128);
    }
}

function healthRank(value) {
    if (value === "CRITICAL") return 2;
    if (value === "WARN") return 1;
    return 0;
}

function health() {
    let power = "OK";
    if (state.battery_percent < 35) power = "WARN";
    if (state.battery_percent < 25) power = "CRITICAL";

    let thermal = "OK";
    if (state.bus_temp_c > 38) thermal = "WARN";
    if (state.bus_temp_c > 45) thermal = "CRITICAL";

    const attitude = state.attitude_lock ? "OK" : "WARN";
    const storage = state.storage.used_packets > 900 ? "WARN" : "OK";
    const overall = [power, thermal, attitude, storage].sort((a, b) => healthRank(b) - healthRank(a))[0];

    return { overall, power, thermal, attitude, storage };
}

function snapshot() {
    return {
        ...state,
        health: health(),
        storage_percent: Math.round((state.storage.used_packets / state.storage.capacity_packets) * 1000) / 10,
    };
}

function writePackets(packets) {
    const free = state.storage.capacity_packets - state.storage.used_packets;
    const accepted = Math.min(Math.max(0, packets), free);
    state.storage.used_packets += accepted;
    state.storage.dropped_packets += packets - accepted;
    return accepted;
}

function readPackets(packets) {
    const removed = Math.min(state.storage.used_packets, Math.max(0, packets));
    state.storage.used_packets -= removed;
    return removed;
}

function payloadAllowed() {
    return state.mode !== "SAFE" && state.battery_percent >= 35 && state.bus_temp_c <= 38;
}

function forceSafe(reason) {
    if (state.mode !== "SAFE") {
        event("ERROR", "safety", reason);
    }
    state.mode = "SAFE";
    state.payload_active = false;
    state.downlink_active = false;
}

function step() {
    state.tick += 1;
    state.solar_input_percent = 62 + ((state.tick * 7) % 29);

    if (state.solar_input_percent > 55 && state.battery_percent < 100) {
        state.battery_percent += 1;
    }
    if ((state.payload_active || state.downlink_active) && state.battery_percent > 0) {
        state.battery_percent -= 1;
    }

    if (state.payload_active || state.downlink_active) {
        state.bus_temp_c += 1;
    } else if (state.bus_temp_c > 12) {
        state.bus_temp_c -= 1;
    }

    state.attitude_lock = state.mode !== "BOOT";
    state.radiation_counts = 100 + ((state.tick * 7) % 50);
    state.camera_temp_c = 12 + (state.tick % 8);
    state.image_ready = state.tick % 3 === 0;

    if (state.payload_active) {
        const packets = 3 + (state.image_ready ? 1 : 0);
        const accepted = writePackets(packets);
        if (accepted < packets) {
            event("WARN", "storage", "payload packets dropped");
        }
    }

    if (state.downlink_active) {
        state.frames_sent += readPackets(2);
    }

    if (state.battery_percent < 25) forceSafe("battery below safe threshold");
    if (state.bus_temp_c > 45) forceSafe("thermal limit exceeded");
}

function applyCommand(rawCommand) {
    const command = String(rawCommand || "").trim().toLowerCase();
    if (!validCommands.has(command)) {
        state.rejected_commands += 1;
        event("WARN", "command", `unknown command: ${rawCommand}`);
        return snapshot();
    }

    event("INFO", "command", command);

    if (command === "reset") {
        state = newState();
        event("INFO", "command", "reset complete");
        return snapshot();
    }
    if (command === "step") {
        step();
        return snapshot();
    }
    if (command === "safe") {
        state.mode = "SAFE";
        state.payload_active = false;
        state.downlink_active = false;
    }
    if (command === "nominal") {
        state.mode = "NOMINAL";
    }
    if (command === "science") {
        if (payloadAllowed()) {
            state.mode = "SCIENCE";
            state.payload_active = true;
        } else {
            state.rejected_commands += 1;
            event("WARN", "safety", "science command rejected");
        }
    }
    if (command === "downlink") {
        if (state.battery_percent >= 30) {
            state.mode = "DOWNLINK";
            state.downlink_active = true;
        } else {
            state.rejected_commands += 1;
            event("WARN", "safety", "downlink command rejected");
        }
    }
    if (command === "stop_payload") {
        state.payload_active = false;
        if (state.mode === "SCIENCE") state.mode = "NOMINAL";
    }

    step();
    return snapshot();
}

function sendJson(response, payload) {
    const body = JSON.stringify(payload, null, 2);
    response.writeHead(200, {
        "Content-Type": "application/json; charset=utf-8",
        "Content-Length": Buffer.byteLength(body),
        "Cache-Control": "no-store",
    });
    response.end(body);
}

function readBody(request) {
    return new Promise((resolve) => {
        let data = "";
        request.on("data", (chunk) => {
            data += chunk;
        });
        request.on("end", () => {
            try {
                resolve(JSON.parse(data || "{}"));
            } catch {
                resolve({});
            }
        });
    });
}

function contentType(filePath) {
    if (filePath.endsWith(".html")) return "text/html; charset=utf-8";
    if (filePath.endsWith(".css")) return "text/css; charset=utf-8";
    if (filePath.endsWith(".js")) return "application/javascript; charset=utf-8";
    return "application/octet-stream";
}

function serveStatic(request, response) {
    const requestPath = new URL(request.url, `http://${host}:${port}`).pathname;
    const safePath = requestPath === "/" ? "/index.html" : requestPath;
    const filePath = path.normalize(path.join(staticRoot, safePath));

    if (!filePath.startsWith(staticRoot)) {
        response.writeHead(403);
        response.end("Forbidden");
        return;
    }

    fs.readFile(filePath, (error, data) => {
        if (error) {
            response.writeHead(404);
            response.end("Not found");
            return;
        }
        response.writeHead(200, {
            "Content-Type": contentType(filePath),
            "Cache-Control": "no-store",
        });
        response.end(data);
    });
}

const server = http.createServer(async (request, response) => {
    const url = new URL(request.url, `http://${host}:${port}`);

    if (request.method === "GET" && url.pathname === "/api/state") {
        sendJson(response, snapshot());
        return;
    }
    if (request.method === "GET" && url.pathname === "/api/events") {
        sendJson(response, { events: state.events.slice(-64) });
        return;
    }
    if (request.method === "GET" && url.pathname === "/api/commands") {
        sendJson(response, { commands: Array.from(validCommands).sort() });
        return;
    }
    if (request.method === "POST" && url.pathname === "/api/command") {
        const body = await readBody(request);
        sendJson(response, applyCommand(body.command));
        return;
    }
    if (request.method === "POST" && url.pathname === "/api/reset") {
        state = newState();
        event("INFO", "command", "reset complete");
        sendJson(response, snapshot());
        return;
    }

    serveStatic(request, response);
});

server.listen(port, host, () => {
    console.log(`Asteria ground station running at http://${host}:${port}`);
});

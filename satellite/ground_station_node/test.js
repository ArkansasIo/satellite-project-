const { spawn } = require("child_process");
const path = require("path");

const port = 8876;
const serverPath = path.join(__dirname, "server.js");
const server = spawn(process.execPath, [serverPath], {
    env: { ...process.env, ASTERIA_PORT: String(port) },
    stdio: ["ignore", "pipe", "pipe"],
});

let serverError = "";
server.stderr.on("data", (chunk) => {
    serverError += chunk.toString();
});

async function json(path, options = {}) {
    const response = await fetch(`http://127.0.0.1:${port}${path}`, {
        headers: { "Content-Type": "application/json" },
        ...options,
    });
    if (!response.ok) {
        throw new Error(`${response.status} ${response.statusText}`);
    }
    return response.json();
}

async function waitForServer() {
    const deadline = Date.now() + 5000;
    while (Date.now() < deadline) {
        if (server.exitCode !== null) {
            throw new Error(`server exited early: ${serverError}`);
        }
        try {
            await json("/api/state");
            return;
        } catch {
            await new Promise((resolve) => setTimeout(resolve, 100));
        }
    }
    throw new Error(`server did not start: ${serverError}`);
}

async function main() {
    await waitForServer();
    let state = await json("/api/state");
    if (state.mode !== "BOOT") throw new Error("initial mode should be BOOT");

    state = await json("/api/command", {
        method: "POST",
        body: JSON.stringify({ command: "nominal" }),
    });
    if (state.mode !== "NOMINAL") throw new Error("nominal command failed");

    state = await json("/api/command", {
        method: "POST",
        body: JSON.stringify({ command: "science" }),
    });
    if (!state.payload_active) throw new Error("science command did not activate payload");

    const events = await json("/api/events");
    if (!Array.isArray(events.events) || events.events.length === 0) {
        throw new Error("events endpoint returned no events");
    }

    console.log("node ground station tests passed");
}

main()
    .catch((error) => {
        console.error(error);
        process.exitCode = 1;
    })
    .finally(() => {
        server.kill();
    });

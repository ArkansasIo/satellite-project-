const fields = {
    subtitle: document.querySelector("#subtitle"),
    overallHealth: document.querySelector("#overallHealth"),
    mode: document.querySelector("#mode"),
    tick: document.querySelector("#tick"),
    battery: document.querySelector("#battery"),
    temp: document.querySelector("#temp"),
    solar: document.querySelector("#solar"),
    attitude: document.querySelector("#attitude"),
    payload: document.querySelector("#payload"),
    downlink: document.querySelector("#downlink"),
    packets: document.querySelector("#packets"),
    frames: document.querySelector("#frames"),
    radiation: document.querySelector("#radiation"),
    rejected: document.querySelector("#rejected"),
    healthPower: document.querySelector("#healthPower"),
    healthThermal: document.querySelector("#healthThermal"),
    healthAttitude: document.querySelector("#healthAttitude"),
    healthStorage: document.querySelector("#healthStorage"),
    events: document.querySelector("#events"),
};

async function getJson(url, options = {}) {
    const response = await fetch(url, {
        headers: { "Content-Type": "application/json" },
        ...options,
    });
    if (!response.ok) {
        throw new Error(`${response.status} ${response.statusText}`);
    }
    return response.json();
}

function setHealthClass(element, value) {
    element.classList.remove("OK", "WARN", "CRITICAL", "ERROR", "INFO");
    element.classList.add(value);
}

function renderState(state) {
    fields.subtitle.textContent = `${state.name} ${state.callsign} software ${state.software_version}`;
    fields.overallHealth.textContent = state.health.overall;
    setHealthClass(fields.overallHealth, state.health.overall);

    fields.mode.textContent = state.mode;
    fields.tick.textContent = state.tick;
    fields.battery.textContent = `${state.battery_percent}%`;
    fields.temp.textContent = `${state.bus_temp_c}C`;
    fields.solar.textContent = `${state.solar_input_percent}%`;
    fields.attitude.textContent = state.attitude_lock ? "LOCK" : "NONE";
    fields.payload.textContent = state.payload_active ? "ON" : "OFF";
    fields.downlink.textContent = state.downlink_active ? "ON" : "OFF";
    fields.packets.textContent = `${state.storage.used_packets}/${state.storage.capacity_packets}`;
    fields.frames.textContent = state.frames_sent;
    fields.radiation.textContent = state.radiation_counts;
    fields.rejected.textContent = state.rejected_commands;

    fields.healthPower.textContent = state.health.power;
    fields.healthThermal.textContent = state.health.thermal;
    fields.healthAttitude.textContent = state.health.attitude;
    fields.healthStorage.textContent = state.health.storage;
    setHealthClass(fields.healthPower, state.health.power);
    setHealthClass(fields.healthThermal, state.health.thermal);
    setHealthClass(fields.healthAttitude, state.health.attitude);
    setHealthClass(fields.healthStorage, state.health.storage);
}

function renderEvents(payload) {
    fields.events.replaceChildren();
    for (const event of payload.events.slice().reverse()) {
        const row = document.createElement("div");
        row.className = "event";

        const tick = document.createElement("span");
        tick.textContent = `T+${String(event.tick).padStart(4, "0")}`;

        const severity = document.createElement("strong");
        severity.textContent = event.severity;
        severity.className = event.severity;

        const source = document.createElement("span");
        source.textContent = event.source;

        const message = document.createElement("span");
        message.textContent = event.message;

        row.append(tick, severity, source, message);
        fields.events.append(row);
    }
}

async function refresh() {
    const [state, events] = await Promise.all([
        getJson("/api/state"),
        getJson("/api/events"),
    ]);
    renderState(state);
    renderEvents(events);
}

async function sendCommand(command) {
    const state = await getJson("/api/command", {
        method: "POST",
        body: JSON.stringify({ command }),
    });
    renderState(state);
    await refresh();
}

document.querySelectorAll("[data-command]").forEach((button) => {
    button.addEventListener("click", () => sendCommand(button.dataset.command));
});

document.querySelector("#refreshButton").addEventListener("click", refresh);

document.querySelector("#resetButton").addEventListener("click", async () => {
    const state = await getJson("/api/reset", { method: "POST" });
    renderState(state);
    await refresh();
});

document.querySelector("#customCommand").addEventListener("submit", async (event) => {
    event.preventDefault();
    const input = document.querySelector("#commandInput");
    const command = input.value.trim();
    if (command.length > 0) {
        await sendCommand(command);
        input.value = "";
    }
});

refresh();
setInterval(refresh, 3000);

from __future__ import annotations

from collections.abc import Iterable
from threading import Lock

from .models import EventRecord, MissionMode, MissionState


VALID_COMMANDS = {
    "safe",
    "nominal",
    "science",
    "downlink",
    "stop_payload",
    "reset",
    "step",
}


class SatelliteSimulator:
    def __init__(self) -> None:
        self._lock = Lock()
        self.state = MissionState()
        self._event("INFO", "boot", "ground station simulator initialized")

    def snapshot(self) -> dict:
        with self._lock:
            return self.state.to_dict()

    def events(self) -> list[dict]:
        with self._lock:
            return [event.__dict__.copy() for event in self.state.events[-64:]]

    def reset(self) -> dict:
        with self._lock:
            self.state = MissionState()
            self._event("INFO", "command", "reset")
            return self.state.to_dict()

    def command(self, command_text: str) -> dict:
        command = command_text.strip().lower()
        with self._lock:
            if command not in VALID_COMMANDS:
                self.state.rejected_commands += 1
                self._event("WARN", "command", f"unknown command: {command_text}")
                return self.state.to_dict()

            self._event("INFO", "command", command)

            if command == "reset":
                self.state = MissionState()
                self._event("INFO", "command", "reset complete")
                return self.state.to_dict()

            if command == "step":
                self._step_locked()
                return self.state.to_dict()

            self._apply_command_locked(command)
            self._step_locked()
            return self.state.to_dict()

    def run_script(self, commands: Iterable[str]) -> dict:
        last = self.snapshot()
        for command in commands:
            stripped = command.strip()
            if not stripped or stripped.startswith("#"):
                continue
            last = self.command(stripped)
        return last

    def _apply_command_locked(self, command: str) -> None:
        if command == "safe":
            self.state.mode = MissionMode.SAFE
            self.state.payload_active = False
            self.state.downlink_active = False
            return

        if command == "nominal":
            self.state.mode = MissionMode.NOMINAL
            return

        if command == "science":
            if not self._payload_allowed_locked():
                self.state.rejected_commands += 1
                self._event("WARN", "safety", "science command rejected")
                return
            self.state.mode = MissionMode.SCIENCE
            self.state.payload_active = True
            return

        if command == "downlink":
            if self.state.battery_percent < 30:
                self.state.rejected_commands += 1
                self._event("WARN", "safety", "downlink command rejected")
                return
            self.state.mode = MissionMode.DOWNLINK
            self.state.downlink_active = True
            return

        if command == "stop_payload":
            self.state.payload_active = False
            if self.state.mode == MissionMode.SCIENCE:
                self.state.mode = MissionMode.NOMINAL

    def _step_locked(self) -> None:
        state = self.state
        state.tick += 1

        state.solar_input_percent = 62 + ((state.tick * 7) % 29)
        if state.solar_input_percent > 55 and state.battery_percent < 100:
            state.battery_percent += 1
        if (state.payload_active or state.downlink_active) and state.battery_percent > 0:
            state.battery_percent -= 1

        if state.payload_active or state.downlink_active:
            state.bus_temp_c += 1
        elif state.bus_temp_c > 12:
            state.bus_temp_c -= 1

        state.attitude_lock = state.mode != MissionMode.BOOT
        state.radiation_counts = 100 + ((state.tick * 7) % 50)
        state.camera_temp_c = 12 + (state.tick % 8)
        state.image_ready = state.tick % 3 == 0

        if state.payload_active:
            packets = 3 + (1 if state.image_ready else 0)
            accepted = state.storage.write(packets)
            if accepted < packets:
                self._event("WARN", "storage", "payload packets dropped")

        if state.downlink_active:
            sent = state.storage.read(2)
            if sent:
                state.frames_sent += sent

        if state.battery_percent < 25:
            self._force_safe_locked("battery below safe threshold")
        if state.bus_temp_c > 45:
            self._force_safe_locked("thermal limit exceeded")

    def _payload_allowed_locked(self) -> bool:
        state = self.state
        return state.mode != MissionMode.SAFE and state.battery_percent >= 35 and state.bus_temp_c <= 38

    def _force_safe_locked(self, reason: str) -> None:
        if self.state.mode != MissionMode.SAFE:
            self._event("ERROR", "safety", reason)
        self.state.mode = MissionMode.SAFE
        self.state.payload_active = False
        self.state.downlink_active = False

    def _event(self, severity: str, source: str, message: str) -> None:
        self.state.events.append(EventRecord(self.state.tick, severity, source, message))
        if len(self.state.events) > 128:
            self.state.events = self.state.events[-128:]


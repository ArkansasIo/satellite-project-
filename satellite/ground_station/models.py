from __future__ import annotations

from dataclasses import asdict, dataclass, field
from enum import Enum
from typing import Any


class MissionMode(str, Enum):
    BOOT = "BOOT"
    SAFE = "SAFE"
    NOMINAL = "NOMINAL"
    SCIENCE = "SCIENCE"
    DOWNLINK = "DOWNLINK"


class HealthStatus(str, Enum):
    OK = "OK"
    WARN = "WARN"
    CRITICAL = "CRITICAL"


@dataclass
class StorageState:
    capacity_packets: int = 1024
    used_packets: int = 0
    dropped_packets: int = 0

    def write(self, packets: int) -> int:
        packets = max(0, packets)
        free_packets = self.capacity_packets - self.used_packets
        accepted = min(packets, free_packets)
        self.used_packets += accepted
        self.dropped_packets += packets - accepted
        return accepted

    def read(self, packets: int) -> int:
        packets = max(0, packets)
        removed = min(self.used_packets, packets)
        self.used_packets -= removed
        return removed


@dataclass
class EventRecord:
    tick: int
    severity: str
    source: str
    message: str


@dataclass
class MissionState:
    name: str = "Asteria-1"
    callsign: str = "AST1"
    software_version: str = "0.2.0"
    tick: int = 0
    mode: MissionMode = MissionMode.BOOT
    battery_percent: int = 78
    bus_temp_c: int = 18
    solar_input_percent: int = 66
    attitude_lock: bool = False
    payload_active: bool = False
    downlink_active: bool = False
    rejected_commands: int = 0
    frames_sent: int = 0
    radiation_counts: int = 100
    camera_temp_c: int = 12
    image_ready: bool = False
    storage: StorageState = field(default_factory=StorageState)
    events: list[EventRecord] = field(default_factory=list)

    def to_dict(self) -> dict[str, Any]:
        data = asdict(self)
        data["mode"] = self.mode.value
        data["health"] = self.health()
        data["storage_percent"] = round((self.storage.used_packets / self.storage.capacity_packets) * 100, 1)
        return data

    def health(self) -> dict[str, str]:
        power = HealthStatus.OK
        if self.battery_percent < 35:
            power = HealthStatus.WARN
        if self.battery_percent < 25:
            power = HealthStatus.CRITICAL

        thermal = HealthStatus.OK
        if self.bus_temp_c > 38:
            thermal = HealthStatus.WARN
        if self.bus_temp_c > 45:
            thermal = HealthStatus.CRITICAL

        attitude = HealthStatus.OK if self.attitude_lock else HealthStatus.WARN
        storage = HealthStatus.WARN if self.storage.used_packets > 900 else HealthStatus.OK
        overall = max([power, thermal, attitude, storage], key=lambda status: _health_rank(status))

        return {
            "overall": overall.value,
            "power": power.value,
            "thermal": thermal.value,
            "attitude": attitude.value,
            "storage": storage.value,
        }


def _health_rank(status: HealthStatus) -> int:
    if status == HealthStatus.CRITICAL:
        return 2
    if status == HealthStatus.WARN:
        return 1
    return 0


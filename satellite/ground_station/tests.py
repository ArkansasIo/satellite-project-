from __future__ import annotations

from .models import MissionMode
from .simulation import SatelliteSimulator


def check(name: str, condition: bool) -> None:
    if not condition:
        raise AssertionError(name)


def test_nominal_science_downlink() -> None:
    sim = SatelliteSimulator()
    sim.command("nominal")
    sim.command("science")
    state = sim.command("downlink")
    check("downlink mode", state["mode"] == MissionMode.DOWNLINK.value)
    check("storage nonnegative", state["storage"]["used_packets"] >= 0)
    check("frames sent", state["frames_sent"] >= 0)


def test_unknown_command_rejected() -> None:
    sim = SatelliteSimulator()
    state = sim.command("not-a-command")
    check("reject count", state["rejected_commands"] == 1)


def test_safe_blocks_science() -> None:
    sim = SatelliteSimulator()
    sim.command("safe")
    state = sim.command("science")
    check("safe remains safe", state["mode"] == MissionMode.SAFE.value)
    check("payload off", state["payload_active"] is False)
    check("rejected", state["rejected_commands"] == 1)


def run() -> None:
    test_nominal_science_downlink()
    test_unknown_command_rejected()
    test_safe_blocks_science()
    print("ground station tests passed")


if __name__ == "__main__":
    run()

# Architecture

Asteria-1 is split into small C modules with explicit subsystem ownership.

- `mission` owns the mission state and top-level update step.
- `commands` converts text commands into state transitions.
- `subsystems` updates simplified power, thermal, attitude, payload, and communications behavior.
- `telemetry` formats operator-visible frames.
- `health` evaluates the current state into OK/WARN/CRITICAL statuses.
- `event_log` records a small in-memory circular log.
- `scheduler` provides a fixed-period cooperative task runner.
- `drivers` contains simulated hardware interfaces.

The simulator is intentionally host-buildable. Hardware-facing files under `drivers/` are interfaces and deterministic simulations, not real spacecraft drivers.

# Requirements

## Functional

- The system shall initialize into `BOOT`.
- The system shall accept text commands from a command file.
- The system shall emit one telemetry frame after each accepted command step.
- The system shall maintain battery, thermal, attitude, payload, downlink, and packet counters.
- The system shall force safe mode below 25 percent battery.
- The system shall force safe mode above 45 C bus temperature.

## Non-Functional

- The simulator shall build as C99.
- The core library shall not depend on operating-system APIs.
- The simulator executable may use standard C file I/O.
- Tests shall run without network access or external services.

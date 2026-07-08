# Expected Behavior

The simulator should:

1. Start in `BOOT`.
2. Enter `NOMINAL` after the first scripted command.
3. Turn payload on in `SCIENCE`.
4. Store telemetry packets while payload is active.
5. Downlink stored packets in `DOWNLINK`.
6. Turn payload and downlink off in `SAFE`.

Safety behavior:

- Battery below 25 percent forces `SAFE`.
- Bus temperature above 45 C forces `SAFE`.

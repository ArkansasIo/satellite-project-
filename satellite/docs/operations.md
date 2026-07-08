# Operations

## Nominal Startup

1. Start the simulator.
2. Load commands from `sim/commands.txt`.
3. Move from `BOOT` to `NOMINAL`.
4. Enable science collection.
5. Enter downlink mode when packets are available.

## Safe Mode

Safe mode disables payload and downlink activity. The simulator automatically enters safe mode when battery or thermal limits are exceeded.

## Command File

Each non-empty, non-comment line is one command:

- `safe`
- `nominal`
- `science`
- `downlink`
- `stop_payload`

# New Satellite Flight Software

Educational source tree for a fictional small satellite named `Asteria-1`.

This code is a simulator-friendly flight software skeleton. It does not control real spacecraft hardware and should not be used for real mission operations.

## Layout

- `include/` - public headers for subsystems and mission state
- `src/` - C source for the flight loop, telemetry, command handling, and subsystem simulation
- `config/` - mission and hardware configuration files
- `scripts/` - PowerShell helpers for build/run/clean
- `sim/` - sample command inputs and simulation notes
- `tests/` - lightweight test notes and sample expected behavior
- `docs/` - architecture, interface, operations, and requirements notes
- `CMakeLists.txt` - CMake build for simulator and tests

## Build And Run

From the repository root:

```powershell
.\satellite\scripts\build.ps1
.\satellite\scripts\run.ps1
```

The program prints a short mission timeline and simulated telemetry frames.

## GUI And API

Run the ground station UI:

```powershell
.\satellite\scripts\run-gui-node.ps1
```

Then open `http://127.0.0.1:8765`.

Run the GUI/API logic tests:

```powershell
.\satellite\scripts\test-gui-node.ps1
```

From inside `satellite/`, the same commands are available as:

```powershell
npm run start
npm test
```

Run tests:

```powershell
.\satellite\scripts\test.ps1
```

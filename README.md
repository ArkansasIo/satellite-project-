# Voyager 1 Inspired Toy OS

This is a small educational 16-bit x86 assembly boot sector. It prints a Voyager-themed telemetry monitor when run in an emulator.

It is not real spacecraft software and cannot run on or control Voyager 1 or any satellite.

The project also includes a separate Raspberry Pi 6 concept target. That target is ARM64 bare-metal code for a fictional Pi-like board with CPU, GPU, four DDR6 RAM slots, HDD, SSD, M.3 expansion, and BIOS/CMOS-like concepts modeled through Pi-style firmware files.

There is also a complete `satellite/` source tree for a fictional new satellite named `Asteria-1`. It contains C flight-software simulation code, mission config, hardware config, scripts, and test notes.

## Files

- `voyager1_os.asm` - 512-byte first-stage bootloader.
- `stage2.asm` - second-stage Voyager-themed telemetry monitor.
- `build.ps1` - Windows PowerShell build script.
- `run.ps1` - Windows PowerShell build-and-run script for QEMU.
- `clean.ps1` - removes generated files.
- `Makefile` - build/run/clean commands for GNU Make.
- `docs.md` - short description of the project files.
- `pi6\kernel8.S` - ARM64 Raspberry Pi-style concept kernel.
- `pi6\config.txt` - Pi-style firmware boot config for `kernel8.img`.
- `pi6\cmdline.txt` - serial console command line placeholder.
- `pi6\board_profile.md` - fictional board profile for the requested hardware.
- `build-pi6.ps1` - builds the ARM64 Pi concept target.
- `run-pi6-qemu.ps1` - runs the concept kernel in QEMU `virt`, not real Pi hardware.
- `satellite\` - Asteria-1 simulated satellite flight software project.
- `run-satellite.ps1` - top-level helper to run the Asteria-1 simulator.

## Build

Install NASM, then run:

```powershell
.\build.ps1
```

## Run

Install QEMU, then run:

```powershell
.\run.ps1
```

The bootloader loads the second stage. The second stage clears the screen, prints boot messages, then repeats a simulated telemetry stream.

## Manual Build Notes

The image is a concatenation of a 512-byte bootloader and `stage2.asm`, padded to at least 17 sectors. `build.ps1` handles this so the bootloader can read stage 2 from sector 2.

## Raspberry Pi 6 Concept Build

Install an ARM64 bare-metal toolchain that provides `aarch64-none-elf-as`, `aarch64-none-elf-ld`, and `aarch64-none-elf-objcopy`, then run:

```powershell
.\build-pi6.ps1
```

This creates:

- `build\pi6\kernel8.img`
- `build\pi6\config.txt`
- `build\pi6\cmdline.txt`

For a real Raspberry Pi-style boot partition, copy those files to the FAT boot partition. The QEMU helper uses a generic virtual ARM board:

```powershell
.\run-pi6-qemu.ps1
```

Note: Raspberry Pi boards use boot firmware, EEPROM, and `config.txt` rather than a PC BIOS and CMOS setup path. The Pi 6 hardware profile here is fictional unless you replace it with an official board specification.

## New Satellite Simulation

Build and run the Asteria-1 simulator:

```powershell
.\satellite\scripts\build.ps1
.\satellite\scripts\run.ps1
```

The simulator prints mission modes, battery, thermal state, attitude lock, payload state, downlink state, and stored packets.

Shortcut:

```powershell
.\run-satellite.ps1
```

The simulator now reads commands from `satellite\sim\commands.txt` and includes a CMake build plus `asteria-tests`.

The satellite tree also includes docs, simulated drivers, an event log, health checks, a cooperative scheduler, and storage accounting.

Run the simulator tests with:

```powershell
.\satellite\scripts\test.ps1
```

## Satellite GUI And API

Run the Python ground station API and browser UI:
This workspace also includes a no-dependency Node.js runner, and `run-satellite-gui.ps1` uses Node when available.

```powershell
.\run-satellite-gui.ps1
```

Then open:

```text
http://127.0.0.1:8765
```

The UI exposes telemetry, health, event log, command buttons, a custom command box, and reset. The REST API is documented in `satellite\docs\api.md`.

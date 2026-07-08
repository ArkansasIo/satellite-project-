# Project Files

`voyager1_os.asm` is the first-stage boot sector. It is exactly 512 bytes and ends with the `0xAA55` boot signature.

`stage2.asm` is the second-stage monitor loaded at `0000:8000`. It contains the Voyager-themed telemetry loop.

`build.ps1` assembles both stages into `build\voyager1_os.img` and checks that the bootloader is exactly 512 bytes.

`run.ps1` builds the image if needed, then starts it in QEMU.

`clean.ps1` removes generated build output.

`Makefile` provides equivalent `make`, `make run`, and `make clean` commands for environments with GNU Make.

`README.md` contains the quick start.

## Pi 6 Concept Target

`pi6\kernel8.S` is a separate ARM64 bare-metal concept kernel for a fictional Raspberry Pi 6-style board.

`pi6\config.txt` and `pi6\cmdline.txt` are Raspberry Pi-style boot partition files.

`pi6\board_profile.md` records the requested CPU, GPU, DDR6 RAM slots, HDD, SSD, M.3, BIOS, and CMOS concepts. The BIOS/CMOS items are modeled as firmware/config and a future settings block because Raspberry Pi boards do not follow the PC BIOS/CMOS boot model.

`build-pi6.ps1` builds `build\pi6\kernel8.img` when an ARM64 bare-metal toolchain is installed.

## Asteria-1 Satellite Source Tree

`satellite\` contains a separate educational C flight-software simulator for a fictional new satellite.

The simulator includes mission state, commands, telemetry formatting, and simplified power, thermal, attitude, payload, and communications subsystem updates.

Use `satellite\scripts\build.ps1` and `satellite\scripts\run.ps1` from the repository root when GCC or Clang is installed.

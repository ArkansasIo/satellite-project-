# Raspberry Pi 6 Concept Board Profile

This profile describes the fictional hardware requested for the toy OS. It is not an official Raspberry Pi specification.

## Boot Model

Raspberry Pi boards use firmware and boot configuration files rather than a PC BIOS setup screen and CMOS RAM. This project models those concepts as:

- BIOS equivalent: Raspberry Pi boot firmware plus `config.txt`
- CMOS equivalent: a future settings block stored in flash or a small file on the boot partition
- Kernel image: `kernel8.img`, loaded by firmware

## Concept Hardware

- CPU: ARM64 multicore processor
- GPU: firmware/mailbox-controlled graphics processor
- RAM: four DDR6 slots, simulated by the monitor text
- HDD: future SATA-style block device driver
- SSD: future USB/SATA-style block device driver
- M.3: fictional successor/variant expansion slot placeholder
- NVMe/M.2: future PCIe storage driver

## Current Implementation

The current kernel initializes only a stack, parks secondary cores, and prints telemetry-style status over UART. Real RAM training, GPU setup, disk drivers, PCIe enumeration, filesystem loading, and persistent settings are intentionally left as future work.

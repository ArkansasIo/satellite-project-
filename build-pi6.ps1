param(
    [string]$Source = "pi6\kernel8.S",
    [string]$LinkerScript = "pi6\linker.ld",
    [string]$OutputDir = "build\pi6"
)

$ErrorActionPreference = "Stop"

$as = Get-Command aarch64-none-elf-as -ErrorAction SilentlyContinue
$ld = Get-Command aarch64-none-elf-ld -ErrorAction SilentlyContinue
$objcopy = Get-Command aarch64-none-elf-objcopy -ErrorAction SilentlyContinue

if (-not $as -or -not $ld -or -not $objcopy) {
    Write-Error "ARM64 bare-metal tools were not found. Install aarch64-none-elf binutils/GCC and add them to PATH."
}

if (-not (Test-Path $OutputDir)) {
    New-Item -ItemType Directory -Path $OutputDir | Out-Null
}

$object = Join-Path $OutputDir "kernel8.o"
$elf = Join-Path $OutputDir "kernel8.elf"
$image = Join-Path $OutputDir "kernel8.img"

aarch64-none-elf-as $Source -o $object
aarch64-none-elf-ld -T $LinkerScript $object -o $elf
aarch64-none-elf-objcopy -O binary $elf $image

Copy-Item -LiteralPath "pi6\config.txt" -Destination (Join-Path $OutputDir "config.txt") -Force
Copy-Item -LiteralPath "pi6\cmdline.txt" -Destination (Join-Path $OutputDir "cmdline.txt") -Force

Write-Host "Built $image"
Write-Host "Copy kernel8.img, config.txt, and cmdline.txt to a Raspberry Pi-style FAT boot partition."

param(
    [string]$Kernel = "build\pi6\kernel8.img"
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path $Kernel)) {
    & "$PSScriptRoot\build-pi6.ps1"
}

if (-not (Get-Command qemu-system-aarch64 -ErrorAction SilentlyContinue)) {
    Write-Error "qemu-system-aarch64 was not found on PATH. Install QEMU, then run this script again."
}

qemu-system-aarch64 -M virt -cpu cortex-a72 -nographic -kernel $Kernel

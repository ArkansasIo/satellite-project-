param(
    [string]$Image = "build\voyager1_os.img"
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path $Image)) {
    & "$PSScriptRoot\build.ps1"
}

if (-not (Get-Command qemu-system-i386 -ErrorAction SilentlyContinue)) {
    Write-Error "qemu-system-i386 was not found on PATH. Install QEMU, then run this script again."
}

qemu-system-i386 -drive "format=raw,file=$Image"

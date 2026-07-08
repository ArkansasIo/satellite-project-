$ErrorActionPreference = "Stop"

if (Test-Path "build") {
    Remove-Item -LiteralPath "build" -Recurse -Force
    Write-Host "Removed build directory."
} else {
    Write-Host "Nothing to clean."
}

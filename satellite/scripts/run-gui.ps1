$ErrorActionPreference = "Stop"

$root = Resolve-Path (Join-Path $PSScriptRoot "..\..")
Set-Location $root

$python = & "$PSScriptRoot\python-command.ps1"
& $python -m satellite.ground_station.api

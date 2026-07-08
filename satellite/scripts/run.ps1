$ErrorActionPreference = "Stop"

$root = Resolve-Path (Join-Path $PSScriptRoot "..")
$cmakeExe = Join-Path $root "build\cmake\Debug\asteria-sim.exe"
$cmakeSingleConfigExe = Join-Path $root "build\cmake\asteria-sim.exe"
$fallbackExe = Join-Path $root "build\asteria-sim.exe"
$commandFile = Join-Path $root "sim\commands.txt"

if (-not (Test-Path $cmakeExe) -and -not (Test-Path $cmakeSingleConfigExe) -and -not (Test-Path $fallbackExe)) {
    & "$PSScriptRoot\build.ps1"
}

if (Test-Path $cmakeExe) {
    & $cmakeExe $commandFile
} elseif (Test-Path $cmakeSingleConfigExe) {
    & $cmakeSingleConfigExe $commandFile
} else {
    & $fallbackExe $commandFile
}

$ErrorActionPreference = "Stop"

$root = Resolve-Path (Join-Path $PSScriptRoot "..")
$cmake = Get-Command cmake -ErrorAction SilentlyContinue

if ($cmake) {
    $buildDir = Join-Path $root "build\cmake"
    cmake -S $root -B $buildDir
    if ($LASTEXITCODE -eq 0) {
        cmake --build $buildDir
        if ($LASTEXITCODE -eq 0) {
            Write-Host "Built CMake project in $buildDir"
            exit 0
        }
    }
    Write-Host "CMake build was not available. Trying direct GCC/Clang build..."
}

$cc = Get-Command gcc -ErrorAction SilentlyContinue
if (-not $cc) {
    $cc = Get-Command clang -ErrorAction SilentlyContinue
}
if (-not $cc) {
    Write-Error "No CMake, GCC, or Clang found. Install one of them and add it to PATH."
}

$outDir = Join-Path $root "build"
if (-not (Test-Path $outDir)) {
    New-Item -ItemType Directory -Path $outDir | Out-Null
}

$exe = Join-Path $outDir "asteria-sim.exe"
$sources = @(
    Join-Path $root "src\main.c",
    Join-Path $root "src\mission.c",
    Join-Path $root "src\commands.c",
    Join-Path $root "src\event_log.c",
    Join-Path $root "src\health.c",
    Join-Path $root "src\scheduler.c",
    Join-Path $root "src\storage.c",
    Join-Path $root "src\subsystems.c",
    Join-Path $root "src\telemetry.c",
    Join-Path $root "src\drivers\adc.c",
    Join-Path $root "src\drivers\radio.c",
    Join-Path $root "src\drivers\sensor.c"
)
$include = Join-Path $root "include"

& $cc.Source -std=c99 -Wall -Wextra -pedantic -I $include @sources -o $exe

Write-Host "Built $exe"

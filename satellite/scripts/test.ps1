$ErrorActionPreference = "Stop"

$root = Resolve-Path (Join-Path $PSScriptRoot "..")
$cmake = Get-Command cmake -ErrorAction SilentlyContinue

if ($cmake) {
    $buildDir = Join-Path $root "build\cmake"
    cmake -S $root -B $buildDir
    if ($LASTEXITCODE -ne 0) {
        Write-Error "CMake configure failed."
    }
    cmake --build $buildDir
    if ($LASTEXITCODE -ne 0) {
        Write-Error "CMake build failed."
    }
    ctest --test-dir $buildDir --output-on-failure
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Tests failed."
    }
    exit 0
}

Write-Error "CMake was not found on PATH. Install CMake and a C compiler to run tests."

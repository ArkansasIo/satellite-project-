$ErrorActionPreference = "Stop"

$root = Resolve-Path (Join-Path $PSScriptRoot "..")
$outDir = Join-Path $root "build"

if (Test-Path $outDir) {
    Remove-Item -LiteralPath $outDir -Recurse -Force
    Write-Host "Removed $outDir"
} else {
    Write-Host "Nothing to clean."
}

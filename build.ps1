param(
    [string]$BootSource = "voyager1_os.asm",
    [string]$Stage2Source = "stage2.asm",
    [string]$Output = "build\voyager1_os.img"
)

$ErrorActionPreference = "Stop"

if (-not (Get-Command nasm -ErrorAction SilentlyContinue)) {
    Write-Error "NASM was not found on PATH. Install NASM, then run this script again."
}

$buildDir = Split-Path -Parent $Output
if ($buildDir -and -not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

$bootBin = Join-Path $buildDir "boot.bin"
$stage2Bin = Join-Path $buildDir "stage2.bin"

nasm -f bin $BootSource -o $bootBin
nasm -f bin $Stage2Source -o $stage2Bin

$boot = Get-Item $bootBin
if ($boot.Length -ne 512) {
    Write-Error "Bootloader should be exactly 512 bytes, but was $($boot.Length) bytes."
}

$outStream = [System.IO.File]::Open($Output, [System.IO.FileMode]::Create, [System.IO.FileAccess]::Write)
try {
    foreach ($part in @($bootBin, $stage2Bin)) {
        $bytes = [System.IO.File]::ReadAllBytes((Resolve-Path $part))
        $outStream.Write($bytes, 0, $bytes.Length)
    }
    $minimumImageSize = 17 * 512
    if ($outStream.Length -lt $minimumImageSize) {
        $outStream.SetLength($minimumImageSize)
    }
} finally {
    $outStream.Close()
}

$image = Get-Item $Output
Write-Host "Built $Output ($($image.Length) bytes)"

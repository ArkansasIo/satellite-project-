$python = Get-Command python -ErrorAction SilentlyContinue
if (-not $python) {
    Write-Error "Python was not found. Install Python 3 and add it to PATH."
}

if ($python.Source -like "*\WindowsApps\python.exe") {
    Write-Error "The Windows Python app execution alias is active, but Python is not installed. Install Python 3 from python.org or disable the alias and add Python to PATH."
}

$python.Source

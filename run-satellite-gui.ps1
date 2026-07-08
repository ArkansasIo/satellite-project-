$ErrorActionPreference = "Stop"

$node = Get-Command node -ErrorAction SilentlyContinue
if ($node) {
    & "$PSScriptRoot\satellite\scripts\run-gui-node.ps1"
} else {
    & "$PSScriptRoot\satellite\scripts\run-gui.ps1"
}

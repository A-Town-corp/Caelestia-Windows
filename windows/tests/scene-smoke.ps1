param([string]$Package = "$PSScriptRoot/../../build/first-test")
$ErrorActionPreference = 'Stop'
$executable = Join-Path $Package 'bin/caelestia-first-test.exe'
if (-not (Test-Path -LiteralPath $executable)) {
    throw "First-test executable missing: $executable"
}
$result = Start-Process -FilePath $executable -ArgumentList '--smoke-test' -PassThru -Wait -WindowStyle Hidden
if ($result.ExitCode -ne 0) {
    throw "Scene smoke test failed with exit code $($result.ExitCode)"
}
Write-Output 'PASS: QML load, animated drawer, rendered frame and clean shutdown'

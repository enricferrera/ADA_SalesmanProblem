param(
    [string]$Configuration = "Debug",
    [string]$Platform = "x64"
)

$Solution = "$(Split-Path -Parent $MyInvocation.MyCommand.Definition)\GraphApplication.sln"

Write-Host "Building solution: $Solution"
Write-Host "Configuration: $Configuration" -ForegroundColor Cyan
Write-Host "Platform: $Platform" -ForegroundColor Cyan

# Try msbuild directly
$msbuildLocation = Get-Command msbuild -ErrorAction SilentlyContinue
if (!$msbuildLocation) {
    Write-Host "msbuild not found in PATH. Consider opening 'Developer Command Prompt for Visual Studio' or adding msbuild to PATH." -ForegroundColor Yellow
    Write-Host "Try this (Developer Prompt): Open the 'x64 Native Tools Command Prompt for VS 2022' and run this script again from there." -ForegroundColor Yellow
    Write-Host "If you still want the script to try to find msbuild via vswhere, please ensure vswhere is installed and present in PATH." -ForegroundColor Yellow
}

# Build JPEGLib first (safe) if project exists
$jpeglibProj = Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Definition) 'JPEGLib\JPEGLib.vcxproj'
if (Test-Path $jpeglibProj) {
    Write-Host "Building JPEGLib project..." -ForegroundColor Green
    & msbuild $jpeglibProj /t:Rebuild /p:Configuration=$Configuration;Platform=$Platform
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Building JPEGLib failed" -ForegroundColor Red
        exit $LASTEXITCODE
    }
}

# Build main solution
Write-Host "Building GraphApplication solution..." -ForegroundColor Green
& msbuild $Solution /t:Rebuild /p:Configuration=$Configuration;Platform=$Platform
if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed." -ForegroundColor Red
    exit $LASTEXITCODE
}

Write-Host "Build succeeded." -ForegroundColor Green

# Optional: show location of exe if present
$exePath = Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Definition) "x64\$Configuration\GraphApplication.exe" # default expected path
if (Test-Path $exePath) {
    Write-Host "Executable built at: $exePath" -ForegroundColor Green
}
else {
    Write-Host "Executable not found in default expected location. Check platform or configuration (Win32 vs x64)." -ForegroundColor Yellow
}

return 0

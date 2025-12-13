# RobinHood - Development Build Script

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "RobinHood - Development Build Script" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Configuration
$GAME_PATH = "E:\SteamLibrary\steamapps\common\Sky Children of the Light"

# Check if Vulkan SDK is installed
if (-not $env:VULKAN_SDK) {
    Write-Host "ERROR: VULKAN_SDK environment variable not found!" -ForegroundColor Red
    Write-Host "Please install Vulkan SDK and set the VULKAN_SDK environment variable." -ForegroundColor Yellow
    Write-Host "Download from: https://vulkan.lunarg.com/" -ForegroundColor Yellow
    exit 1
}

Write-Host "Vulkan SDK found at: $env:VULKAN_SDK" -ForegroundColor Green
Write-Host "Game path: $GAME_PATH" -ForegroundColor Green
Write-Host ""

# Create build directory if it doesn't exist
if (-not (Test-Path "build")) {
    Write-Host "Creating build directory..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Path "build" | Out-Null
}
Set-Location "build"

# Set Ninja in PATH
$env:PATH = "C:\;$env:PATH"

Write-Host "Configuring CMake with Ninja (Debug mode)..." -ForegroundColor Yellow
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_MAKE_PROGRAM=C:/ninja.exe
if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "ERROR: CMake configuration failed!" -ForegroundColor Red
    Set-Location ..
    exit 1
}

Write-Host ""
Write-Host "Building with Ninja (Debug)..." -ForegroundColor Yellow
cmake --build .
if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "ERROR: Build failed!" -ForegroundColor Red
    Set-Location ..
    exit 1
}

Set-Location ..

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "Build completed successfully!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""

# Check if DLL exists
$dllPath = "build\bin\powrprof.dll"
if (Test-Path $dllPath) {
    Write-Host "Copying DLL to game directory..." -ForegroundColor Yellow
    
    # Check if game path exists
    if (Test-Path $GAME_PATH) {
        Copy-Item $dllPath -Destination $GAME_PATH -Force
        Write-Host "Successfully copied powrprof.dll to:" -ForegroundColor Green
        Write-Host "$GAME_PATH" -ForegroundColor Cyan
    } else {
        Write-Host "WARNING: Game path does not exist!" -ForegroundColor Yellow
        Write-Host "Path: $GAME_PATH" -ForegroundColor Yellow
        Write-Host "DLL not copied. Please update the GAME_PATH variable in dev.ps1" -ForegroundColor Yellow
    }
} else {
    Write-Host "WARNING: powrprof.dll not found at $dllPath" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "Output: build\bin\powrprof.dll" -ForegroundColor Cyan
Write-Host ""

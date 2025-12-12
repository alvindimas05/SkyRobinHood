# RobinHood - CMake Build Script

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "RobinHood - CMake Build Script" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Check if Vulkan SDK is installed
if (-not $env:VULKAN_SDK) {
    Write-Host "ERROR: VULKAN_SDK environment variable not found!" -ForegroundColor Red
    Write-Host "Please install Vulkan SDK and set the VULKAN_SDK environment variable." -ForegroundColor Yellow
    Write-Host "Download from: https://vulkan.lunarg.com/" -ForegroundColor Yellow
    exit 1
}

Write-Host "Vulkan SDK found at: $env:VULKAN_SDK" -ForegroundColor Green
Write-Host ""

# Clean build directory if it exists to avoid cache issues
if (Test-Path "build") {
    Write-Host "Cleaning existing build directory..." -ForegroundColor Yellow
    Remove-Item "build" -Recurse -Force -ErrorAction SilentlyContinue
}

# Create build directory
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build" | Out-Null
}
Set-Location "build"

Write-Host "Configuring CMake..." -ForegroundColor Yellow
cmake .. -G "Visual Studio 17 2022" -A x64
if ($LASTEXITCODE -ne 0) {
    Write-Host ""
    Write-Host "ERROR: CMake configuration failed!" -ForegroundColor Red
    Set-Location ..
    exit 1
}

Write-Host ""
Write-Host "Building Release configuration..." -ForegroundColor Yellow
cmake --build . --config Release
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
Write-Host "Output: build\bin\powrprof.dll" -ForegroundColor Cyan
Write-Host ""

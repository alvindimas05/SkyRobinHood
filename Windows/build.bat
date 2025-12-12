@echo off
echo ========================================
echo RobinHood - CMake Build Script
echo ========================================
echo.

REM Check if Vulkan SDK is installed
if not defined VULKAN_SDK (
    echo ERROR: VULKAN_SDK environment variable not found!
    echo Please install Vulkan SDK and set the VULKAN_SDK environment variable.
    echo Download from: https://vulkan.lunarg.com/
    pause
    exit /b 1
)

echo Vulkan SDK found at: %VULKAN_SDK%
echo.

REM Clean build directory if it exists to avoid cache issues
if exist build (
    echo Cleaning existing build directory...
    rmdir /s /q build
)

REM Create build directory
if not exist build mkdir build
cd build

echo Configuring CMake...
cmake .. -G "Visual Studio 17 2022" -A x64
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo Building Release configuration...
cmake --build . --config Release
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo ========================================
echo Build completed successfully!
echo ========================================
echo Output: build\bin\powrprof.dll
echo.
pause

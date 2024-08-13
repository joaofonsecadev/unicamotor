@echo off
setlocal enabledelayedexpansion

:: Get the directory where the script is located
set "SCRIPT_DIR=%~dp0"
set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

:: Default build type
set "BUILD_TYPE=Debug"

:: Parse command line arguments
:parse_args
if "%~1"=="" goto :done_parsing
if /I "%~1"=="-t" (
    set "BUILD_TYPE=%~2"
    shift
) else if /I "%~1"=="-h" (
    call :print_usage
    exit /b 0
) else (
    echo Invalid option: %~1
    call :print_usage
    exit /b 1
)
shift
goto :parse_args
:done_parsing

:: Validate build type
if /I "%BUILD_TYPE%" == "Debug" goto :valid_build_type
if /I "%BUILD_TYPE%" == "Release" goto :valid_build_type
if /I "%BUILD_TYPE%" == "RelWithDebInfo" goto :valid_build_type
if /I "%BUILD_TYPE%" == "MinSizeRel" goto :valid_build_type
echo Invalid build type: %BUILD_TYPE%
call :print_usage
exit /b 1
:valid_build_type

:: Create build directory
set "BUILD_DIR=%SCRIPT_DIR%\build"

:: Configure with CMake
cmake -G Ninja -DCMAKE_BUILD_TYPE="%BUILD_TYPE%" -B "%BUILD_DIR%" -S "%SCRIPT_DIR%"
if %ERRORLEVEL% neq 0 (
    echo CMake configuration failed.
    exit /b 1
)

:: Build with Ninja
ninja -C "%BUILD_DIR%"
if %ERRORLEVEL% neq 0 (
    echo Ninja build failed.
    exit /b 1
)

echo Build completed for %BUILD_TYPE% configuration
exit /b 0

:print_usage
echo Usage: %~nx0 [-t ^<build_type^>]
echo   -t ^<build_type^>    Specify build type (Debug, Release, RelWithDebInfo, MinSizeRel)
echo   -h                 Show this help message
exit /b

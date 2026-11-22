@echo off
setlocal EnableExtensions DisableDelayedExpansion

rem ---------------------------------------------------------------------------
rem  MEMZ-Safe build script (MSVC / Visual Studio 2022)
rem  - Detects cl.exe and attempts to initialize a VS developer environment
rem  - Compiles src\*.cpp into build\MEMZTroj.Safe.exe
rem  - Supports paths containing spaces and returns correct exit codes
rem ---------------------------------------------------------------------------

set "EXIT_CODE=0"
set "SCRIPT_DIR=%~dp0"
if "%SCRIPT_DIR:~-1%"=="\" set "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

pushd "%SCRIPT_DIR%" >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Failed to access script directory: "%~dp0"
    exit /b 1
)

set "SRC_DIR=%SCRIPT_DIR%\src"
set "BUILD_DIR=%SCRIPT_DIR%\build"
set "OUT_EXE=%BUILD_DIR%\MEMZTroj.Safe.exe"
set "OBJ_DIR=%BUILD_DIR%\obj\\"

echo [*] MEMZ-Safe build
echo     Script : "%SCRIPT_DIR%\build.bat"
echo     Source : "%SRC_DIR%"
echo     Output : "%BUILD_DIR%"

if not exist "%SRC_DIR%\main.cpp"  goto :missing_sources
if not exist "%SRC_DIR%\utils.cpp" goto :missing_sources
if not exist "%SRC_DIR%\payloads.cpp" goto :missing_sources
if not exist "%SRC_DIR%\data.cpp"  goto :missing_sources
if not exist "%SRC_DIR%\memz.h"    goto :missing_sources
if not exist "%SRC_DIR%\data.h"    goto :missing_sources

call :ensure_msvc
if errorlevel 1 goto :fail

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%" >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Failed to create build directory: "%BUILD_DIR%"
    goto :fail
)
if not exist "%OBJ_DIR%" mkdir "%OBJ_DIR%" >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Failed to create object directory: "%OBJ_DIR%"
    goto :fail
)

echo [*] Compiler:
where cl.exe
if errorlevel 1 (
    echo [ERROR] cl.exe became unavailable before compilation.
    goto :fail
)

echo [*] Compiling MEMZTroj.Safe.exe...
cl /nologo /std:c++17 /EHsc /W3 /O2 /DUNICODE /D_UNICODE ^
    "%SRC_DIR%\main.cpp" "%SRC_DIR%\utils.cpp" "%SRC_DIR%\payloads.cpp" "%SRC_DIR%\data.cpp" ^
    /Fo"%OBJ_DIR%" /Fe"%OUT_EXE%" ^
    /link /INCREMENTAL:NO user32.lib gdi32.lib winmm.lib advapi32.lib shell32.lib comctl32.lib
set "CL_EXIT=%ERRORLEVEL%"

if not "%CL_EXIT%"=="0" (
    echo [ERROR] Compilation failed with exit code %CL_EXIT%.
    goto :fail
)

if not exist "%OUT_EXE%" (
    echo [ERROR] Compilation reported success, but executable was not found:
    echo         "%OUT_EXE%"
    goto :fail
)

echo [SUCCESS] Build completed successfully.
echo           Executable: "%OUT_EXE%"
popd >nul 2>&1
endlocal & exit /b 0

:missing_sources
echo [ERROR] Required source files are missing in "%SRC_DIR%".
echo         Expected: main.cpp, utils.cpp, payloads.cpp, data.cpp, memz.h, data.h
goto :fail

:ensure_msvc
where cl.exe >nul 2>&1
if not errorlevel 1 exit /b 0

echo [!] cl.exe was not found in PATH.
echo [*] Attempting to locate Visual Studio developer environment...

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
set "VSDEVCMD="
if exist "%VSWHERE%" (
    for /f "usebackq delims=" %%I in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath 2^>nul`) do (
        if exist "%%~I\Common7\Tools\VsDevCmd.bat" set "VSDEVCMD=%%~I\Common7\Tools\VsDevCmd.bat"
    )
)

if not defined VSDEVCMD (
    for %%P in (
        "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
        "%ProgramFiles%\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat"
        "%ProgramFiles%\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat"
        "%ProgramFiles%\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
        "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
        "%ProgramFiles%\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat"
        "%ProgramFiles%\Microsoft Visual Studio\18\Professional\Common7\Tools\VsDevCmd.bat"
        "%ProgramFiles%\Microsoft Visual Studio\18\Enterprise\Common7\Tools\VsDevCmd.bat"
        "%ProgramFiles%\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat"
    ) do (
        if not defined VSDEVCMD if exist "%%~P" set "VSDEVCMD=%%~P"
    )
)

if defined VSDEVCMD (
    echo [*] Initializing MSVC environment:
    echo     "%VSDEVCMD%"
    call "%VSDEVCMD%" -arch=x64 -host_arch=x64 >nul
    if errorlevel 1 (
        echo [ERROR] VsDevCmd.bat was found but failed to initialize the environment.
        echo         Try running this script from "Developer Command Prompt for VS 2022".
        exit /b 1
    )
)

where cl.exe >nul 2>&1
if errorlevel 1 (
    echo [ERROR] MSVC compiler cl.exe is unavailable.
    echo.
    echo Install Visual Studio 2022 or Build Tools with:
    echo   - Workload: Desktop development with C++
    echo   - Component: MSVC v143 ^(or compatible^) C++ build tools
    echo   - Component: Windows 10/11 SDK
    echo.
    echo Then run this script from:
    echo   - "Developer Command Prompt for VS 2022", or
    echo   - "x64 Native Tools Command Prompt for VS 2022"
    echo.
    echo Standard VsDevCmd.bat locations checked include:
    echo   %%ProgramFiles%%\Microsoft Visual Studio\2022\...\Common7\Tools\VsDevCmd.bat
    echo   %%ProgramFiles(x86^)%%\Microsoft Visual Studio\2022\...\Common7\Tools\VsDevCmd.bat
    exit /b 1
)

exit /b 0

:fail
set "EXIT_CODE=1"
popd >nul 2>&1
endlocal & exit /b %EXIT_CODE%

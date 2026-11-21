# Working Windows Build Tutorial (`MEMZ-Safe`)

This guide shows how to actually compile **`MEMZ-Safe`** on **Windows 10 / Windows 11** so that `cl.exe`, Windows headers, and Win32 libraries link cleanly without missing-compiler or missing-header errors.

---

## Step 1: Check Prerequisites
You need:
1. **Windows 10 or Windows 11** (64-bit recommended).
2. **Visual Studio 2022** (Community, Professional, Enterprise, or Build Tools) with the **Desktop development with C++** workload installed (which includes MSVC `cl.exe`, the Windows 10/11 SDK, and `VsDevCmd.bat`).

> **Common pitfall:** Running `cl` inside a normal `cmd.exe` or PowerShell window without loading the Visual Studio developer environment often fails with `'cl' is not recognized` or `fatal error C1083: Cannot open include file: 'windows.h'`. Always initialize the MSVC environment first.

---

## Step 2: Prepare Your Folder Structure
Place your `MEMZ-Safe` source files (`main.cpp`, `payloads.cpp`, `utils.cpp`, `data.h`, `data.cpp`, `memz.h`) inside `src\` at the root of your repository:

```text
MEMZ-Safe/
├── src/
│   ├── data.cpp
│   ├── data.h
│   ├── main.cpp
│   ├── memz.h
│   ├── payloads.cpp
│   └── utils.cpp
├── build/          (created automatically during build)
└── build.bat
```

---

## Step 3: Build Using `VsDevCmd.bat` + `cl.exe` (Works From Any CMD or PowerShell)
Even if you open a plain Command Prompt (`cmd.exe`), you can load the VS 2022 x64 environment and compile directly:

```bat
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64
if not exist "build" mkdir "build"
cl /nologo /std:c++17 /EHsc /W3 /O2 /DUNICODE /D_UNICODE ^
    src\main.cpp src\utils.cpp src\payloads.cpp src\data.cpp ^
    /Fe:build\MEMZTroj.Safe.exe /Fo:build\ ^
    /link user32.lib gdi32.lib winmm.lib advapi32.lib shell32.lib comctl32.lib
```

If you use **Visual Studio Build Tools** instead of Community, adjust the path to:
```bat
"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
```

---

## Step 4: Drop-In `build.bat` Script That Automatically Finds VS 2022
Save this as `build.bat` in the root of your `MEMZ-Safe` folder so building works reliably:

```bat
@echo off
setlocal enabledelayedexpansion

if not exist "build" mkdir "build"

where cl >nul 2>nul
if errorlevel 1 (
    set "VSDEVCMD="
    for %%P in (
        "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
        "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat"
        "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\Tools\VsDevCmd.bat"
        "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
    ) do (
        if exist %%P set "VSDEVCMD=%%~P"
    )

    if defined VSDEVCMD (
        call "!VSDEVCMD!" -arch=x64 -host_arch=x64 >nul 2>nul
    )
)

where cl >nul 2>nul
if errorlevel 1 (
    echo [ERROR] cl.exe was not found. Install Visual Studio 2022 with 'Desktop development with C++'.
    exit /b 1
)

echo [*] Building build\MEMZTroj.Safe.exe ...
cl /nologo /std:c++17 /EHsc /W3 /O2 /DUNICODE /D_UNICODE ^
    src\main.cpp src\utils.cpp src\payloads.cpp src\data.cpp ^
    /Fe:build\MEMZTroj.Safe.exe /Fo:build\ ^
    /link user32.lib gdi32.lib winmm.lib advapi32.lib shell32.lib comctl32.lib

if errorlevel 1 (
    echo [ERROR] Build failed. Check the compiler output above.
    exit /b 1
)

echo [OK] Built build\MEMZTroj.Safe.exe successfully.
endlocal
```

---

## Step 5: Verify the Build Output
After running `build.bat`, verify that the executable was actually produced:

```bat
dir build\MEMZTroj.Safe.exe
```

If `build\MEMZTroj.Safe.exe` exists and `build.bat` exited with code `0`, the build succeeded.

---

## Step 6: Troubleshooting Common Build Errors
- **`'cl' is not recognized as an internal or external command`:** You didn't run from a VS Developer Prompt and `VsDevCmd.bat` wasn't found. Re-run the Visual Studio 2022 Installer and ensure *Desktop development with C++* is checked.
- **`LNK2019: unresolved external symbol __imp__PlaySoundA...` or `BitBlt` / `CryptGenRandom`:** Make sure all required import libraries are passed after `/link`: `user32.lib`, `gdi32.lib`, `winmm.lib`, `advapi32.lib`, `shell32.lib`, `comctl32.lib`.
- **`C1083: Cannot open include file: 'windows.h'`:** The Windows SDK is missing or not initialized. Open the **Visual Studio Installer**, click **Modify**, and ensure a **Windows 10/11 SDK** component is selected under *Desktop development with C++*.
- **Antivirus blocks or quarantines the compiled `.exe` during testing:** Desktop GDI calls (`BitBlt`, `StretchBlt`, `SetCursorPos`, `SendInput`) can trigger heuristic alerts. Exclude your local `build\` test folder in Windows Security while testing on a safe machine/VM, and always keep `Ctrl + Alt + Q` ready.

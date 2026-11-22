# Windows Build Tutorial (MEMZ-Safe)

This guide explains how to build **MEMZ-Safe** on Windows 10/11 using the included `build.bat` script.

## 1. Prerequisites

1. **Windows 10 or Windows 11** (64-bit recommended).
2. **Visual Studio 2022** (Community, Professional, Enterprise, or Build Tools).
3. Install the **Desktop development with C++** workload.
4. Ensure these components are available:
   - MSVC v143 (or compatible) C++ build tools
   - Windows 10/11 SDK

## 2. Project Layout

The project should contain:

```text
MEMZTroj-Safe/
├── .gitignore
├── build.bat
├── CONTRIBUTING.md
├── LICENSE
├── README.md
├── STORY.md
├── docs/
│   ├── BUILD_TUTORIAL.md
│   └── SAFETY_AND_ABORT.md
└── src/
    ├── data.cpp
    ├── data.h
    ├── main.cpp
    ├── memz.h
    ├── payloads.cpp
    └── utils.cpp
```

## 3. Build with `build.bat` (Recommended)

Open `cmd.exe`, Developer Command Prompt for VS 2022, or x64 Native Tools Command Prompt, then run:

```bat
cd /d "path\to\MEMZTroj-Safe"
build.bat
```

### What the script does

- Uses `cd /d "%~dp0"` to switch to the script directory.
- Checks whether `cl.exe` is available.
- If needed, looks for `vswhere.exe` and standard Visual Studio installation paths, then initializes the MSVC environment.
- Retries environment detection/initialization on failure (limited attempts).
- Creates `build\` and `build\obj\`.
- Compiles `src\main.cpp`, `src\utils.cpp`, `src\payloads.cpp`, and `src\data.cpp` into `build\MEMZTroj.Safe.exe`.
- Writes output to `build\build.log`.
- Shows clear errors and does not close itself on error or completion.

## 4. Verify the Build

```bat
dir build\MEMZTroj.Safe.exe
```

Check `build\build.log` for full compiler output.

## 5. Manual Build (Optional)

From a Developer Command Prompt for VS 2022:

```bat
if not exist build\obj mkdir build\obj
cl /nologo /std:c++17 /EHsc /W3 /O2 /DUNICODE /D_UNICODE ^
  src\main.cpp src\utils.cpp src\payloads.cpp src\data.cpp ^
  /Fo:build\obj\ /Fe:build\MEMZTroj.Safe.exe ^
  /link /INCREMENTAL:NO user32.lib gdi32.lib winmm.lib advapi32.lib shell32.lib comctl32.lib
```

## 6. Troubleshooting

- **`cl.exe` unavailable:** Install Visual Studio C++ Build Tools / Desktop development with C++ workload, or run from Developer Command Prompt.
- **`C1083: Cannot open include file: 'windows.h'`:** Install/repair Windows SDK in Visual Studio Installer.
- **`LNK2019` unresolved externals:** Keep required libraries: `user32.lib gdi32.lib winmm.lib advapi32.lib shell32.lib comctl32.lib`.
- **Antivirus warning:** The program uses disruptive desktop/input/sound APIs. Build from source, inspect code, test in a VM, and keep **Ctrl + Alt + Q** ready.

## 7. Safety Reminder

MEMZ-Safe is non-destructive by design (no MBR/disk writes, no file deletion, no forced BSOD/reboot), but intentionally disruptive. Use it only in a controlled environment.
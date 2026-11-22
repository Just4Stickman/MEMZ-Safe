# Working Windows Build Tutorial (`MEMZ-Safe`)

This guide explains how to build **`MEMZ-Safe`** on **Windows 10 / Windows 11** using the `build.bat` script included inside `MEMZTroj-Safe.zip` (published in GitHub Releases).

---

## Step 1: Check Prerequisites
1. **Windows 10 or Windows 11** (64-bit recommended).
2. **Visual Studio 2022** (Community, Professional, Enterprise, or Build Tools) with the **Desktop development with C++** workload installed (provides `cl.exe`, `VsDevCmd.bat`, and the Windows SDK).

---

## Step 2: Extract `MEMZTroj-Safe.zip`
Download `MEMZTroj-Safe.zip` from **Releases** and extract it so you have the `MEMZTroj-Safe/` directory containing:

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

---

## Step 3: Run `build.bat`
Open a terminal (`cmd.exe` or **Developer Command Prompt for VS 2022**), change into the `MEMZTroj-Safe` directory, and run `build.bat`:

```bat
cd MEMZTroj-Safe
build.bat
```

The included `build.bat` script checks for `cl.exe`, attempts to locate and initialize a Visual Studio developer environment (via `vswhere.exe` or standard Visual Studio installation paths) if needed, creates the `build\` and `build\obj\` output folders, and compiles `src/main.cpp`, `src/utils.cpp`, `src/payloads.cpp`, and `src/data.cpp` into `build\MEMZTroj.Safe.exe` linked against `user32.lib`, `gdi32.lib`, `winmm.lib`, `advapi32.lib`, `shell32.lib`, and `comctl32.lib`. If compilation fails, the script reports an error and returns a non-zero exit code.

---

## Step 4: Verify the Build Output
Check that `build\MEMZTroj.Safe.exe` was created and `build.bat` exited with status `0`:

```bat
dir build\MEMZTroj.Safe.exe
```

---

## Step 5: Troubleshooting Common Build Errors
- **`[ERROR] MSVC compiler cl.exe is still unavailable.`:** Visual Studio 2022 with the *Desktop development with C++* workload was not found in the standard paths. Install or modify Visual Studio 2022 to include C++ build tools, or run from a Developer Command Prompt where `cl.exe` is in `PATH`.
- **`LNK2019: unresolved external symbol ...`:** Ensure you did not alter the linker libraries in `build.bat` (`user32.lib`, `gdi32.lib`, `winmm.lib`, `advapi32.lib`, `shell32.lib`, `comctl32.lib`).
- **`C1083: Cannot open include file: 'windows.h'`:** A Windows SDK component is missing in Visual Studio Installer under *Desktop development with C++*.
- **Antivirus flags desktop GDI/input calls during testing:** GDI and input Win32 calls can trigger heuristic alerts. Keep testing in a safe environment/VM and always keep `Ctrl + Alt + Q` ready.

# Building on Windows (Visual Studio 2022)

## Prerequisites
- Windows 10 or Windows 11
- Visual Studio 2022 (with *Desktop development with C++* workload)

## Typical MSVC Build Command
From a **Developer Command Prompt for VS 2022**:

```bat
cl /nologo /std:c++17 /EHsc /W3 /O2 /DUNICODE /D_UNICODE ^
    src\main.cpp src\utils.cpp src\payloads.cpp src\data.cpp ^
    /Fe:build\MEMZTroj.Safe.exe /Fo:build\ ^
    /link user32.lib gdi32.lib winmm.lib advapi32.lib shell32.lib comctl32.lib
```

## Windows 10 / 11 Notes
- Desktop Window Manager (DWM) compositing can affect how direct desktop `HDC` drawing (`BitBlt`, `StretchBlt`, `DrawIcon`) appears compared to older Windows versions.
- Always test visual/input effects safely and keep `Ctrl + Alt + Q` ready for immediate stop/cleanup.

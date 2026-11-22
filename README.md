# MEMZ-Safe

MEMZ-Safe is a **non-destructive** Windows visual and audio recreation inspired by classic MEMZ payload effects. It is intended for educational, research, and visual experimentation in controlled environments.

## Original Creator & Attribution

- **Original MEMZ concept and codebase:** [Leurak](http://danooct1.com/forum/viewtopic.php?f=12&t=470), created for danooct1's *User Made Malware* series.
- **Non-destructive recreation and repository maintenance:** [Just4Stickman](https://github.com/Just4Stickman).

## Safety Guarantees

- **No MBR / disk sector writes:** Never opens `\\.\PhysicalDrive0` and never modifies boot sectors, partition tables, or file systems.
- **No file deletion or system destruction:** Does not delete files, corrupt system files, trigger forced BSODs, or force shutdown/reboot.
- **No persistence / privilege escalation:** Does not add startup entries, services, scheduled tasks, or privilege escalation.
- **Instant kill-switch:** Press **`Ctrl + Alt + Q`** at any time (including during warnings, confirmation, or active effects) to stop effects immediately, restore/redraw the desktop, and exit cleanly.
- **Two-stage warning + typed confirmation:** Requires two warning dialogs and typing `run MEMZTroj.Safe` before clicking **Go**.

> **Note:** Although non-destructive, the effects are intentionally disruptive (screen glitches, cursor movement, popups, sounds, random key input, window text changes, and opening benign sites/tools). Save your work first and test only in a safe environment, preferably a VM.

## Download / Release

The **GitHub Release ZIP already includes the finished prebuilt executable**: `build\MEMZTroj.Safe.exe`.  
You can download the release ZIP and run the included EXE in a controlled environment, or you can build it yourself from source.

## Build It Yourself (Optional)

If you prefer to compile from source, run `build.bat` from the project folder in a **Developer Command Prompt for VS 2022** (or any command prompt where MSVC is available / can be initialized):

```bat
build.bat
```

`build.bat` checks for `cl.exe`, attempts to locate/initialize Visual Studio via `vswhere.exe` or standard paths, compiles the sources, and writes output to `build\build.log`.

Manual compile example:

```bat
cl /nologo /std:c++17 /EHsc /W3 /O2 /DUNICODE /D_UNICODE ^
  src\main.cpp src\utils.cpp src\payloads.cpp src\data.cpp ^
  /Fe:build\MEMZTroj.Safe.exe /Fo:build\obj\ ^
  /link /INCREMENTAL:NO user32.lib gdi32.lib winmm.lib advapi32.lib shell32.lib comctl32.lib
```

See [`docs/BUILD_TUTORIAL.md`](docs/BUILD_TUTORIAL.md) for detailed build steps.

## Repository Contents

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

## Documentation

- [`STORY.md`](STORY.md) - Background and project history.
- [`docs/BUILD_TUTORIAL.md`](docs/BUILD_TUTORIAL.md) - Detailed Windows build tutorial.
- [`docs/SAFETY_AND_ABORT.md`](docs/SAFETY_AND_ABORT.md) - Safety model and kill-switch details.
- [`CONTRIBUTING.md`](CONTRIBUTING.md) - Contribution guidelines.

## License

MIT License. See [`LICENSE`](LICENSE).

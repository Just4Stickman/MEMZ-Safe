# MEMZ-Safe

A non-destructive Windows visual and audio recreation of classic MEMZ payload effects for educational, research, and visual experimentation purposes.

## Original Creator & Attribution

- **Original MEMZ Trojan Concept & Codebase:** Created by **Leurak** (originally shared for danooct1's *User Made Malware* series and forum thread: [http://danooct1.com/forum/viewtopic.php?f=12&t=470](http://danooct1.com/forum/viewtopic.php?f=12&t=470)).
- **Non-Destructive Recreation & Repository:** Maintained by **Just4Stickman** ([https://github.com/Just4Stickman](https://github.com/Just4Stickman)).

## Releases & `MEMZTroj-Safe.zip`

- The `MEMZTroj-Safe.zip` archive is published under **Releases** on GitHub and contains the `MEMZTroj-Safe/` project files (`build.bat`, `README.md`, `.gitignore`, and `src/`).

## Safety Guarantees

- **No MBR / disk sector writes:** Never opens `\\.\PhysicalDrive0` or modifies boot sectors, partition tables, or file systems.
- **No file deletion or system destruction:** Never deletes files, corrupts system files, triggers forced BSODs (`NtRaiseHardError`), forces reboots (`ExitWindowsEx`), or spawns watchdog crash loops.
- **Instant abort kill-switch:** Press **`Ctrl + Alt + Q`** at any time (even during warnings, confirmation dialogs, or active visual/audio loops) to immediately stop all effects, restore/redraw the desktop, and exit cleanly without harm.
- **Two-stage warning + typed confirmation:** Requires explicit agreement on two English warning dialogs plus typing `run MEMZTroj.Safe` before clicking **Go**.

## Build Instructions (Windows / Visual Studio 2022)

Run `build.bat` from the `MEMZTroj-Safe/` folder in a **Developer Command Prompt for VS 2022** (or x64/x86 Native Tools Command Prompt):

```bat
build.bat
```

This compiles `src/*.cpp` into `build\MEMZTroj.Safe.exe` using `cl.exe`. See `BUILD_TUTORIAL.md` for full step-by-step instructions.

## Repository Contents

- `README.md` — Project overview, attribution, releases note for `MEMZTroj-Safe.zip`, safety guarantees, and build guide.
- `STORY.md` — Background and history of MEMZ and how this non-destructive recreation is structured.
- `BUILD_TUTORIAL.md` — Step-by-step Windows build tutorial using the included `build.bat` script.
- `SAFETY_AND_ABORT.md` — Technical notes on removed destructive routines, consent dialogs, and the `Ctrl + Alt + Q` kill-switch.
- `CONTRIBUTING.md` — Guidelines for safe, non-destructive contributions.
- `LICENSE` — MIT License.
- `.gitignore` — Ignore rules for build artifacts and Visual Studio files.

# MEMZTroj.Safe — GitHub Repository Pack

Repository documentation, background story, contributor/attribution notes, and project templates for a non-destructive visual and audio recreation of classic MEMZ payload effects.

> **Note:** This package is a standalone repository documentation and setup bundle without the `MEMZ` source code or binary payload files.

## Original Creator & Attribution
- **Original MEMZ Trojan Concept & Codebase:** Created by **Leurak** (originally shared for danooct1's *User Made Malware* series and forum community: `http://danooct1.com/forum/viewtopic.php?f=12&t=470`).
- **Non-Destructive Repository Pack & Adaptation:** Maintained by **Racer** (`https://github.com/Just4Stickman`).

## What's Inside This Pack
- `README.md` — Project overview, attribution to the original creator, safety notes, and repository structure.
- `STORY.md` — The story and background of MEMZ, its origin in malware showcase culture, and why this safe visual/audio adaptation removes destructive routines.
- `docs/SAFETY_AND_ABORT.md` — Details on removing MBR overwrites and file deletion/system destruction, two-step warnings + typed confirmation (`run MEMZTroj.Safe`), and the `Ctrl + Alt + Q` instant kill-switch.
- `docs/BUILD_WINDOWS.md` — Notes on building Windows C++/Win32 visual/audio projects with Visual Studio 2022 (`cl.exe` / `MSBuild`).
- `CONTRIBUTING.md` — Guidelines for keeping contributions safe, educational, and non-destructive.
- `LICENSE` — MIT License template for the repository.
- `.gitignore` — Standard ignore rules for Windows / Visual Studio / build artifacts.

## Safety Principles
- **No MBR / disk sector writes:** Never touches `\\.\PhysicalDrive0`, boot sectors, or partition tables.
- **No file deletion or system crash/reboot:** Never deletes user/system files, triggers forced BSODs, or forces reboots.
- **Instant kill-switch (`Ctrl + Alt + Q`):** Designed to immediately halt visual/audio effects, restore desktop visuals, and exit cleanly without harm.

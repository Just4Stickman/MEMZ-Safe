# MEMZ-Safe — Documentation, Story & Working Build Guide

Documentation package for **`MEMZ-Safe`**, a non-destructive Windows visual and audio recreation of classic MEMZ payload effects.

> **Note:** This package provides documentation, background history, safety rules, and working Windows build instructions/scripts—without duplicating the `MEMZ-Safe` source archive itself.

## Original Creator & Attribution
- **Original MEMZ Trojan Concept & Codebase:** Created by **Leurak** (shared for danooct1's *User Made Malware* series and forum thread: `http://danooct1.com/forum/viewtopic.php?f=12&t=470`).
- **Non-Destructive Recreation & Repository (`MEMZ-Safe`):** Maintained by **Racer** (`https://github.com/Just4Stickman`).

## Package Contents
- `README.md` — Overview, attribution, safety summary, and quick links to guides.
- `STORY.md` — Detailed, easy-to-follow story of MEMZ, its origins, why it became iconic, and how the non-destructive approach works.
- `docs/BUILD_TUTORIAL.md` — Step-by-step, working Windows build tutorial using Visual Studio 2022 (`cl.exe` / `VsDevCmd.bat` / `MSBuild` / `g++` fallback) and troubleshooting for real-world build errors.
- `docs/SAFETY_AND_ABORT.md` — Safety guarantees (no MBR write, no file deletion, no forced BSOD/reboot), two-stage English warnings, typed confirmation (`run MEMZTroj.Safe`), and the `Ctrl + Alt + Q` kill-switch.
- `CONTRIBUTING.md` — Contribution rules for keeping the repository safe and well-documented.
- `LICENSE` — MIT License.
- `.gitignore` — Git ignore rules for builds, Visual Studio artifacts, and binaries.

## Safety Guarantees
- **No MBR / disk sector writes:** Never opens `\\.\PhysicalDrive0` or writes to boot sectors, partition tables, or raw disk blocks.
- **No file deletion or system crash/reboot:** Never deletes files, triggers forced BSODs (`NtRaiseHardError`), forces reboots (`ExitWindowsEx`), or spawns crash watchdogs.
- **Two-stage warning + typed confirmation:** Shows two English warnings before starting, followed by a confirmation prompt requiring `run MEMZTroj.Safe` before **Go** becomes active.
- **Instant kill-switch (`Ctrl + Alt + Q`):** Stops active effects, purges sound, triggers a desktop redraw, and exits immediately without harm.

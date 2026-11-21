# Safety & Abort Architecture

## Removed Destructive Capabilities
Any non-destructive adaptation built around this repository concept must exclude:
- **MBR / Disk Sector Overwrites:** No opening or writing to `\\.\PhysicalDrive0` or raw disk sectors.
- **File Deletion & Destructive File Writes:** No deleting files, wiping directories, or modifying critical system files.
- **Forced Crashes / Reboots / Watchdogs:** No `NtRaiseHardError` BSOD triggers, forced `ExitWindowsEx` reboots, or aggressive process-kill/watchdog loops.

## User Consent & Instant Kill-Switch
- **Two-Stage English Warnings:** Inform the user clearly before any effect starts that files are not deleted and the MBR is not overwritten, while noting that visual/audio effects can still be disruptive.
- **Typed Confirmation:** Require typing `run MEMZTroj.Safe` before enabling the **Go** action.
- **Global Kill-Switch (`Ctrl + Alt + Q`):** Allow immediate termination of active visual/audio threads, audio purge, and desktop redraw/restore without system damage.

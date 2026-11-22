# Safety & Kill-Switch Architecture

## 1. Strictly Non-Destructive Scope
- **No MBR or disk sector writes:** Never opens `\\.\PhysicalDrive0` or writes to boot sectors, partition tables, or raw disk offsets.
- **No file deletion or destructive file operations:** Never deletes files, wipes folders, or modifies system files.
- **No forced crash, BSOD, or reboot:** Never calls `NtRaiseHardError` to force a BSOD or `ExitWindowsEx` with forced reboot/shutdown flags.

## 2. User Consent Flow
1. **Warning 1 (English):** Explains that no files are deleted and the MBR is not overwritten, notes potential disruption from visual/audio effects, states the **`Ctrl + Alt + Q`** kill-switch, and asks `Yes` / `No`.
2. **Warning 2 (English):** Reiterates non-destructive guarantees, notes continuous effects until stopped, states **`Ctrl + Alt + Q`**, and asks `Yes` / `No`.
3. **Typed Confirmation (`run MEMZTroj.Safe`):** Requires typing `run MEMZTroj.Safe` before **Go** is enabled.
4. **Declining or Canceling:** Displays `"Nothing was changed or harmed. Stay safe online :)"` with an **OK** button and exits cleanly.

## 3. Instant Kill-Switch (`Ctrl + Alt + Q`)
Pressing **`Ctrl + Alt + Q`** at any time sets the abort flag, stops active audio (`SND_PURGE`), triggers a desktop redraw, and exits immediately without harm.

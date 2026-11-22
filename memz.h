#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <shellapi.h>
#include <mmsystem.h>
#include <commctrl.h>
#include <wincrypt.h>
#include <atomic>
#include <vector>

#pragma comment(linker,"\"/manifestdependency:type='win32' "\
"name='Microsoft.Windows.Common-Controls' version='6.0.0.0' "\
"processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

extern std::atomic<bool> g_AbortRequested;
extern std::atomic<bool> g_EffectsActive;
extern int g_ScreenW;
extern int g_ScreenH;

// Kill-switch hotkey: Ctrl + Alt + Q
constexpr UINT KILLSWITCH_MODS = MOD_CONTROL | MOD_ALT;
constexpr UINT KILLSWITCH_VK   = 'Q';

int SecureRandomInt();
void ReverseWideStringInPlace(LPWSTR str);
void TriggerInstantAbortAndExit(int exitCode = 0);
void RestoreDesktopVisuals();
DWORD WINAPI GlobalHotkeyWatcherThread(LPVOID lpParam);

// Non-destructive visual & audio payloads
DWORD WINAPI PayloadBlinkThread(LPVOID lpParam);
DWORD WINAPI PayloadPuzzleThread(LPVOID lpParam);
DWORD WINAPI PayloadTunnelThread(LPVOID lpParam);
DWORD WINAPI PayloadCursorJitterThread(LPVOID lpParam);
DWORD WINAPI PayloadErrorIconsThread(LPVOID lpParam);
DWORD WINAPI PayloadSoundThread(LPVOID lpParam);
DWORD WINAPI PayloadPopupsThread(LPVOID lpParam);
DWORD WINAPI PayloadReverseTextThread(LPVOID lpParam);
DWORD WINAPI PayloadRandomKeysThread(LPVOID lpParam);
DWORD WINAPI PayloadOpenSitesThread(LPVOID lpParam);

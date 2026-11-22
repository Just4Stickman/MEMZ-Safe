#include "memz.h"

#include <cstdlib>

namespace {
HCRYPTPROV AcquireCryptProvider() {
    HCRYPTPROV prov = 0;
    if (CryptAcquireContextW(&prov, nullptr, nullptr, PROV_RSA_FULL, CRYPT_SILENT | CRYPT_VERIFYCONTEXT)) {
        return prov;
    }
    return 0;
}

HCRYPTPROV GetCryptProvider() {
    static HCRYPTPROV prov = AcquireCryptProvider();
    return prov;
}
}

int SecureRandomInt() {
    unsigned int raw = 0;
    HCRYPTPROV prov = GetCryptProvider();
    if (prov != 0 && CryptGenRandom(prov, sizeof(raw), reinterpret_cast<BYTE*>(&raw))) {
        return static_cast<int>(raw & 0x7fffffff);
    }
    return rand() & 0x7fffffff;
}

void ReverseWideStringInPlace(LPWSTR str) {
    if (!str) return;
    int len = lstrlenW(str);
    if (len <= 1) return;

    for (int i = 0, j = len - 1; i < j; ++i, --j) {
        WCHAR tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
    }

    for (int i = 0; i < len - 1; ++i) {
        if (str[i] == L'\n' && str[i + 1] == L'\r') {
            str[i] = L'\r';
            str[i + 1] = L'\n';
        }
    }
}

void RestoreDesktopVisuals() {
    HWND desktop = GetDesktopWindow();
    RedrawWindow(desktop, nullptr, nullptr,
        RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
    InvalidateRect(nullptr, nullptr, TRUE);
    UpdateWindow(desktop);
}

void TriggerInstantAbortAndExit(int exitCode) {
    bool expected = false;
    if (!g_AbortRequested.compare_exchange_strong(expected, true)) {
        return;
    }
    g_EffectsActive.store(false);
    PlaySoundW(nullptr, nullptr, SND_PURGE);
    RestoreDesktopVisuals();
    ExitProcess(static_cast<UINT>(exitCode));
}

DWORD WINAPI GlobalHotkeyWatcherThread(LPVOID lpParam) {
    (void)lpParam;
    const bool registered = (RegisterHotKey(nullptr, 1, KILLSWITCH_MODS, KILLSWITCH_VK) != FALSE);

    MSG msg;
    while (!g_AbortRequested.load()) {
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_HOTKEY && msg.wParam == 1) {
                TriggerInstantAbortAndExit(0);
            }
        }

        // Fallback polling in case a modal dialog blocks message dispatch
        bool ctrlDown = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
        bool altDown  = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
        bool qDown    = (GetAsyncKeyState('Q') & 0x8000) != 0;
        if (ctrlDown && altDown && qDown) {
            TriggerInstantAbortAndExit(0);
        }

        Sleep(15);
    }

    if (registered) {
        UnregisterHotKey(nullptr, 1);
    }
    return 0;
}

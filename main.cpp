#include "memz.h"

std::atomic<bool> g_AbortRequested{ false };
std::atomic<bool> g_EffectsActive{ false };
int g_ScreenW = 0;
int g_ScreenH = 0;

static constexpr int IDC_CONFIRM_EDIT = 1001;
static constexpr int IDC_GO_BUTTON    = 1002;
static constexpr int IDC_CANCEL_BTN   = 1003;

struct ConfirmDialogState {
    bool confirmedGo = false;
    HFONT hFont = nullptr;
};

static void TrimAndCompareConfirmText(HWND hEdit, HWND hGoBtn) {
    wchar_t text[256] = { 0 };
    GetWindowTextW(hEdit, text, 255);
    bool match = (lstrcmpW(text, L"run MEMZTroj.Safe") == 0);
    EnableWindow(hGoBtn, match ? TRUE : FALSE);
}

static LRESULT CALLBACK ConfirmWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    ConfirmDialogState* state = reinterpret_cast<ConfirmDialogState*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));

    switch (uMsg) {
    case WM_CREATE: {
        CREATESTRUCTW* cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
        state = reinterpret_cast<ConfirmDialogState*>(cs->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(state));

        LOGFONTW lf = { 0 };
        GetObjectW(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONTW), &lf);
        lf.lfHeight = -15;
        state->hFont = CreateFontIndirectW(&lf);

        const wchar_t* infoText =
            L"FINAL CONFIRMATION (NON-DESTRUCTIVE MODE)\r\n"
            L"- Does NOT overwrite MBR or disk sectors.\r\n"
            L"- Does NOT delete files or trigger forced BSOD/reboot.\r\n"
            L"- Visual and sound effects will run until stopped.\r\n"
            L"- Kill-switch (abort everything immediately without harm): Ctrl + Alt + Q\r\n\r\n"
            L"Type: run MEMZTroj.Safe";

        HWND hLabel = CreateWindowExW(0, L"STATIC", infoText,
            WS_CHILD | WS_VISIBLE,
            16, 14, 446, 145, hwnd, nullptr, cs->hInstance, nullptr);

        HWND hEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_TABSTOP,
            16, 166, 446, 26, hwnd, reinterpret_cast<HMENU>(static_cast<INT_PTR>(IDC_CONFIRM_EDIT)), cs->hInstance, nullptr);

        HWND hGo = CreateWindowExW(0, L"BUTTON", L"Go",
            WS_CHILD | WS_VISIBLE | WS_DISABLED | BS_DEFPUSHBUTTON | WS_TABSTOP,
            258, 206, 96, 30, hwnd, reinterpret_cast<HMENU>(static_cast<INT_PTR>(IDC_GO_BUTTON)), cs->hInstance, nullptr);

        HWND hCancel = CreateWindowExW(0, L"BUTTON", L"Cancel",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            366, 206, 96, 30, hwnd, reinterpret_cast<HMENU>(static_cast<INT_PTR>(IDC_CANCEL_BTN)), cs->hInstance, nullptr);

        SendMessageW(hLabel, WM_SETFONT, reinterpret_cast<WPARAM>(state->hFont), TRUE);
        SendMessageW(hEdit, WM_SETFONT, reinterpret_cast<WPARAM>(state->hFont), TRUE);
        SendMessageW(hGo, WM_SETFONT, reinterpret_cast<WPARAM>(state->hFont), TRUE);
        SendMessageW(hCancel, WM_SETFONT, reinterpret_cast<WPARAM>(state->hFont), TRUE);

        SetFocus(hEdit);
        return 0;
    }
    case WM_COMMAND: {
        int ctrlId = LOWORD(wParam);
        int notif  = HIWORD(wParam);
        if (ctrlId == IDC_CONFIRM_EDIT && notif == EN_CHANGE && state) {
            HWND hEdit = GetDlgItem(hwnd, IDC_CONFIRM_EDIT);
            HWND hGo   = GetDlgItem(hwnd, IDC_GO_BUTTON);
            TrimAndCompareConfirmText(hEdit, hGo);
        } else if (ctrlId == IDC_GO_BUTTON && state) {
            HWND hEdit = GetDlgItem(hwnd, IDC_CONFIRM_EDIT);
            wchar_t text[256] = { 0 };
            GetWindowTextW(hEdit, text, 255);
            if (lstrcmpW(text, L"run MEMZTroj.Safe") == 0) {
                state->confirmedGo = true;
                DestroyWindow(hwnd);
            }
        } else if (ctrlId == IDC_CANCEL_BTN) {
            DestroyWindow(hwnd);
        }
        return 0;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        if (state && state->hFont) {
            DeleteObject(state->hFont);
            state->hFont = nullptr;
        }
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
}

static bool RunTypedConfirmationWindow(HINSTANCE hInstance) {
    const wchar_t* clsName = L"MEMZTrojSafeConfirmClass";
    WNDCLASSEXW wc = { 0 };
    wc.cbSize        = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc   = ConfirmWndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = clsName;
    RegisterClassExW(&wc);

    ConfirmDialogState state;
    int x = (g_ScreenW > 490) ? (g_ScreenW - 490) / 2 : 50;
    int y = (g_ScreenH > 290) ? (g_ScreenH - 290) / 2 : 50;

    HWND hwnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_DLGMODALFRAME, clsName,
        L"MEMZTroj.Safe - Confirmation Required",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        x, y, 490, 290, nullptr, nullptr, hInstance, &state);

    if (!hwnd) return false;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        if (g_AbortRequested.load()) {
            return false;
        }
        if (!IsDialogMessageW(hwnd, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    return state.confirmedGo;
}

static void LaunchNonDestructivePayloads() {
    g_EffectsActive.store(true);

    LPTHREAD_START_ROUTINE routines[] = {
        PayloadBlinkThread,
        PayloadPuzzleThread,
        PayloadTunnelThread,
        PayloadCursorJitterThread,
        PayloadErrorIconsThread,
        PayloadSoundThread,
        PayloadPopupsThread,
        PayloadReverseTextThread,
        PayloadRandomKeysThread,
        PayloadOpenSitesThread
    };

    std::vector<HANDLE> threads;
    for (auto fn : routines) {
        if (g_AbortRequested.load()) break;
        HANDLE h = CreateThread(nullptr, 0, fn, nullptr, 0, nullptr);
        if (h) threads.push_back(h);
    }

    while (!g_AbortRequested.load()) {
        Sleep(50);
    }

    for (HANDLE h : threads) {
        WaitForSingleObject(h, 200);
        CloseHandle(h);
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    InitCommonControls();
    g_ScreenW = GetSystemMetrics(SM_CXSCREEN);
    g_ScreenH = GetSystemMetrics(SM_CYSCREEN);

    // Start global kill-switch watcher BEFORE any warnings or actions
    HANDLE hWatcher = CreateThread(nullptr, 0, GlobalHotkeyWatcherThread, nullptr, 0, nullptr);

    const wchar_t* warn1 =
        L"WARNING (1/2) - MEMZTroj.Safe\n\n"
        L"This program is a safe recreation of MEMZ screen, cursor, popup, and sound effects.\n"
        L"It does NOT delete files, does NOT overwrite the MBR, and does NOT force a BSOD or system crash.\n"
        L"However, screen glitches, popups, cursor movement, and sounds can still be disruptive.\n\n"
        L"Kill-switch at any time: Press Ctrl + Alt + Q to abort everything immediately without harm.\n\n"
        L"Do you want to continue?";

    int ans1 = MessageBoxW(nullptr, warn1, L"MEMZTroj.Safe - Warning", MB_YESNO | MB_ICONWARNING | MB_TOPMOST);
    if (g_AbortRequested.load()) {
        TriggerInstantAbortAndExit(0);
    }
    if (ans1 != IDYES) {
        MessageBoxW(nullptr, L"Nothing was changed or harmed. Stay safe online :)", L"MEMZTroj.Safe", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
        if (hWatcher) {
            g_AbortRequested.store(true);
            WaitForSingleObject(hWatcher, 300);
            CloseHandle(hWatcher);
        }
        return 0;
    }

    const wchar_t* warn2 =
        L"WARNING (2/2) - FINAL SAFETY NOTICE\n\n"
        L"Reminder: No files are deleted and no MBR/boot sector is written.\n"
        L"Effects run continuously without auto-removing themselves until stopped.\n\n"
        L"Kill-switch at any time: Press Ctrl + Alt + Q to abort everything immediately without harm.\n\n"
        L"Proceed to typed confirmation?";

    int ans2 = MessageBoxW(nullptr, warn2, L"MEMZTroj.Safe - Second Warning", MB_YESNO | MB_ICONEXCLAMATION | MB_TOPMOST);
    if (g_AbortRequested.load()) {
        TriggerInstantAbortAndExit(0);
    }
    if (ans2 != IDYES) {
        MessageBoxW(nullptr, L"Nothing was changed or harmed. Stay safe online :)", L"MEMZTroj.Safe", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
        if (hWatcher) {
            g_AbortRequested.store(true);
            WaitForSingleObject(hWatcher, 300);
            CloseHandle(hWatcher);
        }
        return 0;
    }

    bool confirmed = RunTypedConfirmationWindow(hInstance);
    if (g_AbortRequested.load()) {
        TriggerInstantAbortAndExit(0);
    }
    if (!confirmed) {
        MessageBoxW(nullptr, L"Nothing was changed or harmed. Stay safe online :)", L"MEMZTroj.Safe", MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
        if (hWatcher) {
            g_AbortRequested.store(true);
            WaitForSingleObject(hWatcher, 300);
            CloseHandle(hWatcher);
        }
        return 0;
    }

    LaunchNonDestructivePayloads();

    if (hWatcher) {
        WaitForSingleObject(hWatcher, 300);
        CloseHandle(hWatcher);
    }
    RestoreDesktopVisuals();
    return 0;
}

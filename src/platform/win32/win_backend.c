#include "win_backend.h"

#include "win_gdi_render.h"
#include "win_input.h"
#include "xile_logger.h"

static HWND g_hwnd;
static int g_should_quit;

static LRESULT CALLBACK xile_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
    case WM_ERASEBKGND:
        return 1;
    case WM_PAINT:
        return win_gdi_render_paint(hwnd);
    case WM_TIMER:
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    case WM_CLOSE:
        return 0;
    case WM_XILE_EXIT:
        g_should_quit = 1;
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }
}

int win_backend_start(const XileConfig *config) {
    WNDCLASSEXW wc;
    HINSTANCE instance = GetModuleHandleW(NULL);
    int x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int y = GetSystemMetrics(SM_YVIRTUALSCREEN);
    int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    (void)config;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = xile_wndproc;
    wc.hInstance = instance;
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wc.lpszClassName = L"XileOverlay";
    if (RegisterClassExW(&wc) == 0 && GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
        return -1;
    }
    g_hwnd = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, L"XileOverlay", L"Xile",
                             WS_POPUP | WS_VISIBLE, x, y, width, height, NULL, NULL, instance, NULL);
    if (g_hwnd == NULL) {
        return -1;
    }
    (void)SetWindowPos(g_hwnd, HWND_TOPMOST, x, y, width, height, SWP_SHOWWINDOW);
    (void)SetWindowDisplayAffinity(g_hwnd, WDA_EXCLUDEFROMCAPTURE);
    if (win_gdi_render_init(g_hwnd) != 0) {
        DestroyWindow(g_hwnd);
        g_hwnd = NULL;
        return -1;
    }
    if (win_input_install_hook(g_hwnd) != 0) {
        win_gdi_render_shutdown();
        DestroyWindow(g_hwnd);
        g_hwnd = NULL;
        return -1;
    }
    (void)SetTimer(g_hwnd, 1u, 16u, NULL);
    XLOG_INFO("win32", "overlay window created");
    return 0;
}

int win_backend_run_loop(void) {
    MSG msg;
    while (!g_should_quit && GetMessageW(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}

int win_backend_stop(void) {
    win_input_uninstall_hook();
    win_gdi_render_shutdown();
    if (g_hwnd != NULL) {
        KillTimer(g_hwnd, 1u);
        DestroyWindow(g_hwnd);
        g_hwnd = NULL;
    }
    g_should_quit = 0;
    return 0;
}

HWND win_backend_hwnd(void) { return g_hwnd; }

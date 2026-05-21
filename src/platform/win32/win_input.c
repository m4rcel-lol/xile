#include "win_input.h"

#include "win_backend.h"
#include "xile_logger.h"

static HHOOK g_keyboard_hook;
static HWND g_target;

static int key_down(DWORD vk) { return (GetAsyncKeyState((int)vk) & 0x8000) != 0; }

static LRESULT CALLBACK keyboard_proc(int code, WPARAM wparam, LPARAM lparam) {
    if (code == HC_ACTION) {
        const KBDLLHOOKSTRUCT *kbd = (const KBDLLHOOKSTRUCT *)lparam;
        int down = wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN;
        if (down) {
            int ctrl = key_down(VK_CONTROL);
            int alt = key_down(VK_MENU);
            int shift = key_down(VK_SHIFT);
            if (kbd->vkCode == VK_LWIN || kbd->vkCode == VK_RWIN) {
                return 1;
            }
            if (ctrl && kbd->vkCode == VK_ESCAPE) {
                return 1;
            }
            if (alt && (kbd->vkCode == VK_F4 || kbd->vkCode == VK_TAB)) {
                return 1;
            }
            if (ctrl && alt && shift && kbd->vkCode == VK_F12) {
                PostMessageW(g_target, WM_XILE_EXIT, 0, 0);
                return 1;
            }
            if (ctrl && alt && kbd->vkCode == VK_DELETE) {
                XLOG_WARN("win32-input", "Ctrl+Alt+Del cannot be suppressed by user-mode hooks");
            }
        }
    }
    return CallNextHookEx(g_keyboard_hook, code, wparam, lparam);
}

int win_input_install_hook(HWND target) {
    g_target = target;
    g_keyboard_hook = SetWindowsHookExW(WH_KEYBOARD_LL, keyboard_proc, GetModuleHandleW(NULL), 0);
    return g_keyboard_hook != NULL ? 0 : -1;
}

void win_input_uninstall_hook(void) {
    if (g_keyboard_hook != NULL) {
        (void)UnhookWindowsHookEx(g_keyboard_hook);
        g_keyboard_hook = NULL;
    }
    g_target = NULL;
}

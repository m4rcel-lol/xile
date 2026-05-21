#include "win_shell_replace.h"

#include <wchar.h>

static int set_shell_value(const wchar_t *value) {
    HKEY key;
    DWORD bytes = (DWORD)((wcslen(value) + 1u) * sizeof(wchar_t));
    LONG rc = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                            L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon", 0,
                            KEY_SET_VALUE, &key);
    if (rc != ERROR_SUCCESS) {
        return -1;
    }
    rc = RegSetValueExW(key, L"Shell", 0, REG_SZ, (const BYTE *)value, bytes);
    RegCloseKey(key);
    return rc == ERROR_SUCCESS ? 0 : -1;
}

int win_shell_replace_enable(const wchar_t *xile_server_path) {
    if (xile_server_path == NULL || xile_server_path[0] == L'\0') {
        return -1;
    }
    return set_shell_value(xile_server_path);
}

int win_shell_replace_disable(void) { return set_shell_value(L"explorer.exe"); }

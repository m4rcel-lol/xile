#include "win_autostart.h"

#include <stdio.h>
#include <windows.h>
#include <wchar.h>

static const wchar_t *RUN_KEY = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";

int win_autostart_enable(const wchar_t *server_path) {
    HKEY key;
    wchar_t value[2048];
    LONG rc;
    if (server_path == NULL || server_path[0] == L'\0') {
        return -1;
    }
    (void)swprintf(value, 2048u, L"\"%ls\" --autostart", server_path);
    rc = RegCreateKeyExW(HKEY_CURRENT_USER, RUN_KEY, 0, NULL, 0, KEY_SET_VALUE, NULL, &key, NULL);
    if (rc != ERROR_SUCCESS) {
        return -1;
    }
    rc = RegSetValueExW(key, L"Xile", 0, REG_SZ, (const BYTE *)value,
                        (DWORD)((wcslen(value) + 1u) * sizeof(wchar_t)));
    RegCloseKey(key);
    return rc == ERROR_SUCCESS ? 0 : -1;
}

int win_autostart_disable(void) {
    HKEY key;
    LONG rc = RegOpenKeyExW(HKEY_CURRENT_USER, RUN_KEY, 0, KEY_SET_VALUE, &key);
    if (rc != ERROR_SUCCESS) {
        return 0;
    }
    rc = RegDeleteValueW(key, L"Xile");
    RegCloseKey(key);
    return rc == ERROR_SUCCESS || rc == ERROR_FILE_NOT_FOUND ? 0 : -1;
}

int win_autostart_is_enabled(void) {
    HKEY key;
    LONG rc = RegOpenKeyExW(HKEY_CURRENT_USER, RUN_KEY, 0, KEY_QUERY_VALUE, &key);
    if (rc != ERROR_SUCCESS) {
        return 0;
    }
    rc = RegQueryValueExW(key, L"Xile", NULL, NULL, NULL, NULL);
    RegCloseKey(key);
    return rc == ERROR_SUCCESS ? 1 : 0;
}

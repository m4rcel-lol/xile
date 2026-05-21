#ifndef XILE_WIN_AUTOSTART_H
#define XILE_WIN_AUTOSTART_H

#include <wchar.h>

int win_autostart_enable(const wchar_t *server_path);
int win_autostart_disable(void);
int win_autostart_is_enabled(void);

#endif

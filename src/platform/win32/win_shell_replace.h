#ifndef XILE_WIN_SHELL_REPLACE_H
#define XILE_WIN_SHELL_REPLACE_H

#ifdef _WIN32
#include <windows.h>
#else
#error "win_shell_replace.h is only valid on Windows"
#endif

int win_shell_replace_enable(const wchar_t *xile_server_path);
int win_shell_replace_disable(void);

#endif

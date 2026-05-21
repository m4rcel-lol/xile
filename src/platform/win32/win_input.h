#ifndef XILE_WIN_INPUT_H
#define XILE_WIN_INPUT_H

#ifdef _WIN32
#include <windows.h>
#else
#error "win_input.h is only valid on Windows"
#endif

int win_input_install_hook(HWND target);
void win_input_uninstall_hook(void);

#endif

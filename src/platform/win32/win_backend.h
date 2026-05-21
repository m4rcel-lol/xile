#ifndef XILE_WIN_BACKEND_H
#define XILE_WIN_BACKEND_H

#include "xile_config.h"

#ifdef _WIN32
#include <windows.h>
#else
#error "win_backend.h is only valid on Windows"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define WM_XILE_EXIT (WM_APP + 0x5849)

int win_backend_start(const XileConfig *config);
int win_backend_run_loop(void);
int win_backend_stop(void);
HWND win_backend_hwnd(void);

#ifdef __cplusplus
}
#endif

#endif

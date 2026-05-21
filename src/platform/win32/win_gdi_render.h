#ifndef XILE_WIN_GDI_RENDER_H
#define XILE_WIN_GDI_RENDER_H

#ifdef _WIN32
#include <windows.h>
#else
#error "win_gdi_render.h is only valid on Windows"
#endif

int win_gdi_render_init(HWND hwnd);
LRESULT win_gdi_render_paint(HWND hwnd);
void win_gdi_render_shutdown(void);

#endif

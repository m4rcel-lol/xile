#ifndef XILE_WIN_D3D_RENDER_H
#define XILE_WIN_D3D_RENDER_H

#ifdef _WIN32
#include <windows.h>
#else
#error "win_d3d_render.h is only valid on Windows"
#endif

int win_d3d_render_available(void);
int win_d3d_render_init(HWND hwnd);
int win_d3d_render_present(void);
void win_d3d_render_shutdown(void);

#endif

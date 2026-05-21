#include "win_d3d_render.h"

int win_d3d_render_available(void) { return 0; }

int win_d3d_render_init(HWND hwnd) { return hwnd != NULL && win_d3d_render_available() ? 0 : -1; }

int win_d3d_render_present(void) { return win_d3d_render_available() ? 0 : -1; }

void win_d3d_render_shutdown(void) {}

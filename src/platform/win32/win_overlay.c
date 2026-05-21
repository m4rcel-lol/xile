#include "win_overlay.h"
#include "win_backend.h"

int win_overlay_is_active(void) { return win_backend_hwnd() != NULL ? 1 : 0; }

#include "xile_ctl_ui.h"

#ifdef _WIN32
#include "win_tray.h"
#elif defined(__APPLE__)
#include "mac_menubar.h"
#else
#error "Unsupported platform"
#endif

int xile_ctl_ui_run(void) {
#ifdef _WIN32
    return win_tray_show();
#elif defined(__APPLE__)
    return mac_menubar_show();
#else
#error "Unsupported platform"
#endif
}

#include "xile_server.h"

#include "xile_input.h"
#include "xile_logger.h"
#include "xile_render.h"
#include "xile_screen.h"

#ifdef _WIN32
#include "win_backend.h"
#elif defined(__APPLE__)
#include "mac_backend.h"
#else
#error "Unsupported platform"
#endif

static int g_running;
static XileConfig g_config;

int xile_server_start_embedded(const XileConfig *config) {
    if (config == NULL || g_running) {
        return -1;
    }
    g_config = *config;
    if (xile_screen_init(1280, 720) != 0) {
        return -1;
    }
    if (xile_render_init(&g_config) != 0) {
        xile_screen_shutdown();
        return -1;
    }
    if (xile_input_init(g_config.exit_chord) != 0) {
        xile_render_shutdown();
        xile_screen_shutdown();
        return -1;
    }
#ifdef _WIN32
    if (win_backend_start(&g_config) != 0) {
        xile_input_shutdown();
        xile_render_shutdown();
        xile_screen_shutdown();
        return -1;
    }
#elif defined(__APPLE__)
    if (mac_backend_start(&g_config) != 0) {
        xile_input_shutdown();
        xile_render_shutdown();
        xile_screen_shutdown();
        return -1;
    }
#else
#error "Unsupported platform"
#endif
    g_running = 1;
    XLOG_INFO("server", "embedded server initialized");
    return 0;
}

int xile_server_run_loop(void) {
    if (!g_running) {
        return -1;
    }
#ifdef _WIN32
    return win_backend_run_loop();
#elif defined(__APPLE__)
    return mac_backend_run_loop();
#else
#error "Unsupported platform"
#endif
}

int xile_server_stop_embedded(void) {
    if (!g_running) {
        return 0;
    }
#ifdef _WIN32
    (void)win_backend_stop();
#elif defined(__APPLE__)
    (void)mac_backend_stop();
#else
#error "Unsupported platform"
#endif
    xile_input_shutdown();
    xile_render_shutdown();
    xile_screen_shutdown();
    g_running = 0;
    XLOG_INFO("server", "embedded server stopped");
    return 0;
}

int xile_server_is_running(void) { return g_running; }

#include "xile_render.h"

#include "xile_logger.h"

static XileRenderBackend g_backend;

int xile_render_init(const XileConfig *config) {
    if (config == NULL) {
        return -1;
    }
    g_backend = config->render_backend;
    XLOG_INFO("render", "selected backend=%s", xile_config_render_backend_name(g_backend));
    return 0;
}

int xile_render_present(void) { return 0; }

void xile_render_shutdown(void) { g_backend = XILE_RENDER_AUTO; }

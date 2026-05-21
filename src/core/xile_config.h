#ifndef XILE_CONFIG_H
#define XILE_CONFIG_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum XileMode {
    XILE_MODE_OVERLAY = 0,
    XILE_MODE_SHELL_REPLACE = 1
} XileMode;

typedef enum XileRenderBackend {
    XILE_RENDER_AUTO = 0,
    XILE_RENDER_GDI = 1,
    XILE_RENDER_D3D11 = 2,
    XILE_RENDER_METAL = 3,
    XILE_RENDER_CGL = 4
} XileRenderBackend;

typedef struct XileConfig {
    XileMode mode;
    int autostart;
    char display_number[16];
    char wm[32];
    char icewm_theme[64];
    char exit_chord[64];
    char log_level[16];
    XileRenderBackend render_backend;
    char config_path[1024];
} XileConfig;

void xile_config_defaults(XileConfig *cfg);
int xile_config_load(XileConfig *cfg, const char *override_path);
int xile_config_parse_file(XileConfig *cfg, const char *path);
int xile_config_parse_bool(const char *value, int *out_value);
const char *xile_config_mode_name(XileMode mode);
const char *xile_config_render_backend_name(XileRenderBackend backend);
int xile_config_write_default_file(const char *path);

#ifdef __cplusplus
}
#endif

#endif

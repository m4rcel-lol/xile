#include "xile_config.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#elif defined(__APPLE__)
#include <pwd.h>
#include <sys/stat.h>
#include <unistd.h>
#else
#error "Unsupported platform"
#endif

static void xile_copy_string(char *dst, size_t dst_len, const char *src) {
    if (dst_len == 0u) {
        return;
    }
    if (src == NULL) {
        dst[0] = '\0';
        return;
    }
    (void)snprintf(dst, dst_len, "%s", src);
}

void xile_config_defaults(XileConfig *cfg) {
    if (cfg == NULL) {
        return;
    }
    memset(cfg, 0, sizeof(*cfg));
    cfg->mode = XILE_MODE_OVERLAY;
    cfg->autostart = 0;
    xile_copy_string(cfg->display_number, sizeof(cfg->display_number), ":1");
    xile_copy_string(cfg->wm, sizeof(cfg->wm), "icewm");
    xile_copy_string(cfg->icewm_theme, sizeof(cfg->icewm_theme), "Xile");
    xile_copy_string(cfg->exit_chord, sizeof(cfg->exit_chord), "ctrl+alt+shift+f12");
    xile_copy_string(cfg->log_level, sizeof(cfg->log_level), "info");
    cfg->render_backend = XILE_RENDER_AUTO;
}

static char *trim(char *s) {
    char *end = NULL;
    while (*s != '\0' && isspace((unsigned char)*s) != 0) {
        ++s;
    }
    if (*s == '\0') {
        return s;
    }
    end = s + strlen(s) - 1u;
    while (end > s && isspace((unsigned char)*end) != 0) {
        *end = '\0';
        --end;
    }
    return s;
}

static void unquote(char *value) {
    size_t len = strlen(value);
    if (len >= 2u && value[0] == '"' && value[len - 1u] == '"') {
        memmove(value, value + 1, len - 2u);
        value[len - 2u] = '\0';
    }
}

static int eq_ci(const char *a, const char *b) {
    while (*a != '\0' && *b != '\0') {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 0;
        }
        ++a;
        ++b;
    }
    return *a == *b;
}

int xile_config_parse_bool(const char *value, int *out_value) {
    if (value == NULL || out_value == NULL) {
        return -1;
    }
    if (eq_ci(value, "true") || eq_ci(value, "1") || eq_ci(value, "yes")) {
        *out_value = 1;
        return 0;
    }
    if (eq_ci(value, "false") || eq_ci(value, "0") || eq_ci(value, "no")) {
        *out_value = 0;
        return 0;
    }
    return -1;
}

static int apply_kv(XileConfig *cfg, const char *key, const char *value) {
    int bool_value = 0;
    if (eq_ci(key, "mode")) {
        if (eq_ci(value, "overlay")) {
            cfg->mode = XILE_MODE_OVERLAY;
            return 0;
        }
        if (eq_ci(value, "shell_replace") || eq_ci(value, "shell-replace")) {
            cfg->mode = XILE_MODE_SHELL_REPLACE;
            return 0;
        }
        return -1;
    }
    if (eq_ci(key, "autostart")) {
        if (xile_config_parse_bool(value, &bool_value) != 0) {
            return -1;
        }
        cfg->autostart = bool_value;
        return 0;
    }
    if (eq_ci(key, "display_number")) {
        xile_copy_string(cfg->display_number, sizeof(cfg->display_number), value);
        return 0;
    }
    if (eq_ci(key, "wm")) {
        if (!eq_ci(value, "icewm")) {
            return -1;
        }
        xile_copy_string(cfg->wm, sizeof(cfg->wm), value);
        return 0;
    }
    if (eq_ci(key, "icewm_theme")) {
        xile_copy_string(cfg->icewm_theme, sizeof(cfg->icewm_theme), value);
        return 0;
    }
    if (eq_ci(key, "exit_chord")) {
        xile_copy_string(cfg->exit_chord, sizeof(cfg->exit_chord), value);
        return 0;
    }
    if (eq_ci(key, "log_level")) {
        if (!(eq_ci(value, "debug") || eq_ci(value, "info") || eq_ci(value, "warn") ||
              eq_ci(value, "error"))) {
            return -1;
        }
        xile_copy_string(cfg->log_level, sizeof(cfg->log_level), value);
        return 0;
    }
    if (eq_ci(key, "render_backend")) {
        if (eq_ci(value, "auto")) {
            cfg->render_backend = XILE_RENDER_AUTO;
        } else if (eq_ci(value, "gdi")) {
            cfg->render_backend = XILE_RENDER_GDI;
        } else if (eq_ci(value, "d3d11")) {
            cfg->render_backend = XILE_RENDER_D3D11;
        } else if (eq_ci(value, "metal")) {
            cfg->render_backend = XILE_RENDER_METAL;
        } else if (eq_ci(value, "cgl")) {
            cfg->render_backend = XILE_RENDER_CGL;
        } else {
            return -1;
        }
        return 0;
    }
    return 0;
}

int xile_config_parse_file(XileConfig *cfg, const char *path) {
    FILE *fp = NULL;
    char line[1024];
    unsigned long line_no = 0;
    if (cfg == NULL || path == NULL) {
        return -1;
    }
    fp = fopen(path, "rb");
    if (fp == NULL) {
        return -1;
    }
    while (fgets(line, sizeof(line), fp) != NULL) {
        char *cursor = NULL;
        char *eq = NULL;
        ++line_no;
        line[strcspn(line, "\r\n")] = '\0';
        cursor = trim(line);
        if (cursor[0] == '\0' || cursor[0] == '#' || cursor[0] == ';') {
            continue;
        }
        if (cursor[0] == '[') {
            continue;
        }
        eq = strchr(cursor, '=');
        if (eq == NULL) {
            (void)fclose(fp);
            return -(int)line_no;
        }
        *eq = '\0';
        {
            char *key = trim(cursor);
            char *value = trim(eq + 1);
            unquote(value);
            if (apply_kv(cfg, key, value) != 0) {
                (void)fclose(fp);
                return -(int)line_no;
            }
        }
    }
    (void)fclose(fp);
    xile_copy_string(cfg->config_path, sizeof(cfg->config_path), path);
    return 0;
}

static void append_path(char *out, size_t out_len, const char *base, const char *tail) {
    (void)snprintf(out, out_len, "%s%s", base, tail);
}

static int user_config_path(char *out, size_t out_len) {
#ifdef _WIN32
    const char *appdata = getenv("APPDATA");
    if (appdata == NULL || appdata[0] == '\0') {
        return -1;
    }
    append_path(out, out_len, appdata, "\\Xile\\xile.ini");
    return 0;
#elif defined(__APPLE__)
    const char *home = getenv("HOME");
    if (home == NULL || home[0] == '\0') {
        struct passwd *pw = getpwuid(getuid());
        if (pw == NULL) {
            return -1;
        }
        home = pw->pw_dir;
    }
    append_path(out, out_len, home, "/Library/Application Support/Xile/xile.ini");
    return 0;
#else
#error "Unsupported platform"
#endif
}

int xile_config_load(XileConfig *cfg, const char *override_path) {
    char user_path[1024];
    if (cfg == NULL) {
        return -1;
    }
    xile_config_defaults(cfg);
#ifdef _WIN32
    (void)xile_config_parse_file(cfg, "C:\\ProgramData\\Xile\\xile.ini");
#elif defined(__APPLE__)
    (void)xile_config_parse_file(cfg, "/Library/Application Support/Xile/xile.ini");
#else
#error "Unsupported platform"
#endif
    if (user_config_path(user_path, sizeof(user_path)) == 0) {
        (void)xile_config_parse_file(cfg, user_path);
    }
    if (override_path != NULL && override_path[0] != '\0') {
        return xile_config_parse_file(cfg, override_path);
    }
    return 0;
}

const char *xile_config_mode_name(XileMode mode) {
    return mode == XILE_MODE_SHELL_REPLACE ? "shell_replace" : "overlay";
}

const char *xile_config_render_backend_name(XileRenderBackend backend) {
    switch (backend) {
    case XILE_RENDER_GDI:
        return "gdi";
    case XILE_RENDER_D3D11:
        return "d3d11";
    case XILE_RENDER_METAL:
        return "metal";
    case XILE_RENDER_CGL:
        return "cgl";
    case XILE_RENDER_AUTO:
    default:
        return "auto";
    }
}

int xile_config_write_default_file(const char *path) {
    FILE *fp = NULL;
    if (path == NULL || path[0] == '\0') {
        return -1;
    }
    fp = fopen(path, "wb");
    if (fp == NULL) {
        return -1;
    }
    (void)fputs("# Xile configuration\n"
                "# mode: overlay or shell_replace\n"
                "mode = overlay\n"
                "# autostart: true or false\n"
                "autostart = false\n"
                "# display_number: X11 display identifier\n"
                "display_number = :1\n"
                "# wm: only icewm is supported in Xile 1.0\n"
                "wm = icewm\n"
                "# icewm_theme: theme directory name\n"
                "icewm_theme = Xile\n"
                "# exit_chord: overlay escape chord\n"
                "exit_chord = ctrl+alt+shift+f12\n"
                "# log_level: debug, info, warn, or error\n"
                "log_level = info\n"
                "# render_backend: auto, gdi, d3d11, metal, or cgl\n"
                "render_backend = auto\n",
                fp);
    return fclose(fp) == 0 ? 0 : -1;
}

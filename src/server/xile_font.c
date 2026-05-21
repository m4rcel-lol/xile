#include "xile_font.h"

#include <stdio.h>
#include <string.h>

static char g_font_root[1024];

int xile_font_configure_paths(const char *font_root) {
    if (font_root == NULL || font_root[0] == '\0' || strlen(font_root) >= sizeof(g_font_root)) {
        return -1;
    }
    (void)snprintf(g_font_root, sizeof(g_font_root), "%s", font_root);
    return 0;
}

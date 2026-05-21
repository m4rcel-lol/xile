#include "xile_screen.h"

#include <stdlib.h>
#include <string.h>

static XileFramebuffer g_fb;

int xile_screen_init(uint32_t width, uint32_t height) {
    size_t bytes = (size_t)width * (size_t)height * 4u;
    if (width == 0u || height == 0u || bytes / 4u / width != height) {
        return -1;
    }
    xile_screen_shutdown();
    g_fb.pixels = (uint8_t *)calloc(1u, bytes);
    if (g_fb.pixels == NULL) {
        return -1;
    }
    g_fb.width = width;
    g_fb.height = height;
    g_fb.stride = width * 4u;
    g_fb.pixel_bytes = bytes;
    g_fb.dirty = 1u;
    return 0;
}

void xile_screen_shutdown(void) {
    free(g_fb.pixels);
    memset(&g_fb, 0, sizeof(g_fb));
}

XileFramebuffer *xile_screen_framebuffer(void) { return g_fb.pixels != NULL ? &g_fb : NULL; }

void xile_screen_mark_dirty(void) { g_fb.dirty = 1u; }

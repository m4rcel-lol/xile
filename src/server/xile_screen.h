#ifndef XILE_SCREEN_H
#define XILE_SCREEN_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct XileFramebuffer {
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    volatile uint32_t dirty;
    uint8_t *pixels;
    size_t pixel_bytes;
} XileFramebuffer;

int xile_screen_init(uint32_t width, uint32_t height);
void xile_screen_shutdown(void);
XileFramebuffer *xile_screen_framebuffer(void);
void xile_screen_mark_dirty(void);

#ifdef __cplusplus
}
#endif

#endif

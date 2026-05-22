#include "mac_cgl_render.h"

#include <stddef.h>
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif
#import <OpenGL/OpenGL.h>

int mac_cgl_render_available(void) {
    CGLPixelFormatAttribute attrs[] = {kCGLPFAAccelerated, kCGLPFADoubleBuffer, 0};
    CGLPixelFormatObj fmt = NULL;
    GLint count = 0;
    CGLError err = CGLChoosePixelFormat(attrs, &fmt, &count);
    if (fmt != NULL) {
        CGLDestroyPixelFormat(fmt);
    }
    return err == kCGLNoError && count > 0 ? 1 : 0;
}

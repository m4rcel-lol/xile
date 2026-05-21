#include "mac_metal_render.h"

#import <Metal/Metal.h>

int mac_metal_render_available(void) {
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    return device != nil ? 1 : 0;
}

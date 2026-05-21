#include "mac_overlay.h"

#import <Cocoa/Cocoa.h>

int mac_overlay_is_supported(void) { return [NSScreen mainScreen] != nil ? 1 : 0; }

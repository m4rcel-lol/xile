#include "mac_input.h"

#import <ApplicationServices/ApplicationServices.h>

static CFMachPortRef gTap;
static CFRunLoopSourceRef gSource;

static CGEventRef tap_callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    (void)proxy;
    (void)refcon;
    if (type == kCGEventKeyDown) {
        CGEventFlags flags = CGEventGetFlags(event);
        int64_t key = CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
        if ((flags & kCGEventFlagMaskCommand) != 0 && (key == 48 || key == 49 || key == 12)) {
            return (CGEventRef)0;
        }
    }
    return event;
}

int mac_input_install_tap(void) {
    CGEventMask mask = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventMouseMoved) |
                       CGEventMaskBit(kCGEventLeftMouseDown) | CGEventMaskBit(kCGEventLeftMouseUp);
    gTap = CGEventTapCreate(kCGAnnotatedSessionEventTap, kCGHeadInsertEventTap, kCGEventTapOptionDefault,
                            mask, tap_callback, NULL);
    if (gTap == NULL) {
        return -1;
    }
    gSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, gTap, 0);
    if (gSource == NULL) {
        CFRelease(gTap);
        gTap = NULL;
        return -1;
    }
    CFRunLoopAddSource(CFRunLoopGetCurrent(), gSource, kCFRunLoopCommonModes);
    CGEventTapEnable(gTap, true);
    return 0;
}

void mac_input_remove_tap(void) {
    if (gSource != NULL) {
        CFRunLoopRemoveSource(CFRunLoopGetCurrent(), gSource, kCFRunLoopCommonModes);
        CFRelease(gSource);
        gSource = NULL;
    }
    if (gTap != NULL) {
        CFRelease(gTap);
        gTap = NULL;
    }
}

#include "mac_backend.h"

#import <ApplicationServices/ApplicationServices.h>
#import <Cocoa/Cocoa.h>

static NSWindow *gWindow;
static BOOL gStopRequested;

int mac_backend_start(const XileConfig *config) {
    (void)config;
    @autoreleasepool {
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyAccessory];
        NSScreen *screen = [NSScreen mainScreen];
        if (screen == nil) {
            return -1;
        }
        gWindow = [[NSWindow alloc] initWithContentRect:[screen frame]
                                               styleMask:NSWindowStyleMaskBorderless
                                                 backing:NSBackingStoreBuffered
                                                   defer:NO];
        [gWindow setLevel:CGShieldingWindowLevel() + 1];
        [gWindow setCollectionBehavior:NSWindowCollectionBehaviorCanJoinAllSpaces |
                                       NSWindowCollectionBehaviorStationary |
                                       NSWindowCollectionBehaviorIgnoresCycle];
        [gWindow setIgnoresMouseEvents:NO];
        [gWindow makeKeyAndOrderFront:nil];
    }
    return 0;
}

int mac_backend_run_loop(void) {
    @autoreleasepool {
        while (!gStopRequested) {
            NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                                untilDate:[NSDate dateWithTimeIntervalSinceNow:0.016]
                                                   inMode:NSDefaultRunLoopMode
                                                  dequeue:YES];
            if (event != nil) {
                [NSApp sendEvent:event];
            }
        }
    }
    return 0;
}

int mac_backend_stop(void) {
    @autoreleasepool {
        gStopRequested = YES;
        [gWindow close];
        gWindow = nil;
    }
    return 0;
}

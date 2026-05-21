#include "mac_menubar.h"

#import <Cocoa/Cocoa.h>

static NSStatusItem *gStatusItem;

int mac_menubar_show(void) {
    gStatusItem = [[NSStatusBar systemStatusBar] statusItemWithLength:NSVariableStatusItemLength];
    if (gStatusItem == nil) {
        return -1;
    }
    [gStatusItem.button setTitle:@"Xile"];
    return 0;
}

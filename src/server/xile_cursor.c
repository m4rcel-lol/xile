#include "xile_cursor.h"

static int g_cursor_x;
static int g_cursor_y;

int xile_cursor_set_position(int x, int y) {
    g_cursor_x = x;
    g_cursor_y = y;
    return 0;
}

void xile_cursor_get_position(int *x, int *y) {
    if (x != 0) {
        *x = g_cursor_x;
    }
    if (y != 0) {
        *y = g_cursor_y;
    }
}

Bool XileCursorOffScreen(ScreenPtr *ppScreen, int *x, int *y) {
    (void)ppScreen;
    (void)x;
    (void)y;
    return 0;
}

void XileCrossingScreen(ScreenPtr pScreen, Bool entering) {
    (void)pScreen;
    (void)entering;
}

void XilePointerMoved(ScreenPtr pScreen, int x, int y) {
    (void)pScreen;
    (void)xile_cursor_set_position(x, y);
}

Bool XileScreenInit(ScreenPtr pScreen, int argc, char **argv) {
    (void)pScreen;
    (void)argc;
    (void)argv;
    return 1;
}

Bool XileCloseScreen(ScreenPtr pScreen) {
    (void)pScreen;
    return 1;
}

Bool XileSaveScreen(ScreenPtr pScreen, int mode) {
    (void)pScreen;
    (void)mode;
    return 1;
}

void XileBlockHandler(ScreenPtr pScreen, void *timeout) {
    (void)pScreen;
    (void)timeout;
}

void XileWakeupHandler(ScreenPtr pScreen, int result) {
    (void)pScreen;
    (void)result;
}

Bool XileSetMode(ScrnInfoPtr pScrn, DisplayModePtr mode) {
    (void)pScrn;
    (void)mode;
    return 1;
}

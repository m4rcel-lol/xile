#ifndef XILE_CURSOR_H
#define XILE_CURSOR_H

#ifdef __cplusplus
extern "C" {
#endif

int xile_cursor_set_position(int x, int y);
void xile_cursor_get_position(int *x, int *y);

typedef int Bool;
typedef void *ScreenPtr;
typedef void *ScrnInfoPtr;
typedef void *DisplayModePtr;

Bool XileCursorOffScreen(ScreenPtr *ppScreen, int *x, int *y);
void XileCrossingScreen(ScreenPtr pScreen, Bool entering);
void XilePointerMoved(ScreenPtr pScreen, int x, int y);
Bool XileScreenInit(ScreenPtr pScreen, int argc, char **argv);
Bool XileCloseScreen(ScreenPtr pScreen);
Bool XileSaveScreen(ScreenPtr pScreen, int mode);
void XileBlockHandler(ScreenPtr pScreen, void *timeout);
void XileWakeupHandler(ScreenPtr pScreen, int result);
Bool XileSetMode(ScrnInfoPtr pScrn, DisplayModePtr mode);

#ifdef __cplusplus
}
#endif

#endif

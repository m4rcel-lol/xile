#include "win_gdi_render.h"

#include "xile_screen.h"

int win_gdi_render_init(HWND hwnd) {
    return hwnd != NULL && xile_screen_framebuffer() != NULL ? 0 : -1;
}

LRESULT win_gdi_render_paint(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(hwnd, &ps);
    XileFramebuffer *fb = xile_screen_framebuffer();
    if (dc != NULL && fb != NULL && fb->pixels != NULL) {
        BITMAPINFO bmi;
        ZeroMemory(&bmi, sizeof(bmi));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = (LONG)fb->width;
        bmi.bmiHeader.biHeight = -(LONG)fb->height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        (void)StretchDIBits(dc, 0, 0, (int)fb->width, (int)fb->height, 0, 0, (int)fb->width,
                            (int)fb->height, fb->pixels, &bmi, DIB_RGB_COLORS, SRCCOPY);
        fb->dirty = 0u;
    }
    EndPaint(hwnd, &ps);
    return 0;
}

void win_gdi_render_shutdown(void) {}

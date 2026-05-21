#ifndef XILE_AUTH_H
#define XILE_AUTH_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define XILE_AUTH_COOKIE_LEN 16u
#define XILE_AUTH_NAME "MIT-MAGIC-COOKIE-1"

typedef struct XileAuthCookie {
    uint8_t bytes[XILE_AUTH_COOKIE_LEN];
    size_t len;
} XileAuthCookie;

int xile_auth_generate(XileAuthCookie *cookie);
int xile_auth_write_xauthority(const char *path, const char *display, const XileAuthCookie *cookie);
int xile_auth_generate_and_write(const char *path, const char *display, XileAuthCookie *cookie);
int xile_auth_verify(const uint8_t *cookie, size_t len);
void xile_auth_forget(void);

#ifdef __cplusplus
}
#endif

#endif

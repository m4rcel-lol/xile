#include "xile_auth.h"

#include <stdio.h>

int main(void) {
    XileAuthCookie cookie;
    if (xile_auth_generate(&cookie) != 0) {
        (void)fprintf(stderr, "cookie generation failed\n");
        return 1;
    }
    if (cookie.len != XILE_AUTH_COOKIE_LEN) {
        (void)fprintf(stderr, "bad cookie length\n");
        return 1;
    }
    if (!xile_auth_verify(cookie.bytes, cookie.len)) {
        (void)fprintf(stderr, "cookie verify failed\n");
        return 1;
    }
    return 0;
}

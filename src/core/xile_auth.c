#include "xile_auth.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <bcrypt.h>
#include <io.h>
#include <sys/stat.h>
#include <windows.h>
#elif defined(__APPLE__)
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#else
#error "Unsupported platform"
#endif

static XileAuthCookie g_cookie;
static int g_cookie_valid;

static void put_be16(FILE *fp, unsigned short value) {
    (void)fputc((int)((value >> 8u) & 0xffu), fp);
    (void)fputc((int)(value & 0xffu), fp);
}

int xile_auth_generate(XileAuthCookie *cookie) {
    if (cookie == NULL) {
        return -1;
    }
    memset(cookie, 0, sizeof(*cookie));
    cookie->len = XILE_AUTH_COOKIE_LEN;
#ifdef _WIN32
    if (BCryptGenRandom(NULL, cookie->bytes, (ULONG)cookie->len,
                        BCRYPT_USE_SYSTEM_PREFERRED_RNG) != 0) {
        return -1;
    }
#elif defined(__APPLE__)
    int fd = open("/dev/urandom", O_RDONLY);
    size_t off = 0u;
    if (fd < 0) {
        return -1;
    }
    while (off < cookie->len) {
        ssize_t n = read(fd, cookie->bytes + off, cookie->len - off);
        if (n <= 0) {
            (void)close(fd);
            return -1;
        }
        off += (size_t)n;
    }
    (void)close(fd);
#else
#error "Unsupported platform"
#endif
    g_cookie = *cookie;
    g_cookie_valid = 1;
    return 0;
}

static const char *display_number_only(const char *display) {
    if (display == NULL || display[0] == '\0') {
        return "1";
    }
    if (display[0] == ':') {
        return display + 1;
    }
    {
        const char *colon = strchr(display, ':');
        return colon != NULL ? colon + 1 : display;
    }
}

int xile_auth_write_xauthority(const char *path, const char *display, const XileAuthCookie *cookie) {
    FILE *fp = NULL;
    const char *host = "localhost";
    const char *display_part = display_number_only(display);
    const char *name = XILE_AUTH_NAME;
    if (path == NULL || display == NULL || cookie == NULL || cookie->len != XILE_AUTH_COOKIE_LEN) {
        return -1;
    }
    fp = fopen(path, "wb");
    if (fp == NULL) {
        return -1;
    }
    put_be16(fp, 256u);
    put_be16(fp, (unsigned short)strlen(host));
    (void)fwrite(host, 1u, strlen(host), fp);
    put_be16(fp, (unsigned short)strlen(display_part));
    (void)fwrite(display_part, 1u, strlen(display_part), fp);
    put_be16(fp, (unsigned short)strlen(name));
    (void)fwrite(name, 1u, strlen(name), fp);
    put_be16(fp, (unsigned short)cookie->len);
    (void)fwrite(cookie->bytes, 1u, cookie->len, fp);
    if (fclose(fp) != 0) {
        return -1;
    }
#ifdef _WIN32
    (void)_chmod(path, _S_IREAD | _S_IWRITE);
#elif defined(__APPLE__)
    (void)chmod(path, 0600);
#else
#error "Unsupported platform"
#endif
    return 0;
}

int xile_auth_generate_and_write(const char *path, const char *display, XileAuthCookie *cookie) {
    XileAuthCookie local_cookie;
    XileAuthCookie *target = cookie != NULL ? cookie : &local_cookie;
    if (xile_auth_generate(target) != 0) {
        return -1;
    }
    return xile_auth_write_xauthority(path, display, target);
}

int xile_auth_verify(const uint8_t *cookie, size_t len) {
    uint8_t diff = 0u;
    if (!g_cookie_valid || cookie == NULL || len != g_cookie.len) {
        return 0;
    }
    for (size_t i = 0u; i < len; ++i) {
        diff = (uint8_t)(diff | (uint8_t)(cookie[i] ^ g_cookie.bytes[i]));
    }
    return diff == 0u ? 1 : 0;
}

void xile_auth_forget(void) {
    memset(&g_cookie, 0, sizeof(g_cookie));
    g_cookie_valid = 0;
}

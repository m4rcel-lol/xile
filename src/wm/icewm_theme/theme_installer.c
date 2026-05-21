#include "theme_installer.h"

#include <stdio.h>

int theme_installer_copy_file(const char *src, const char *dst) {
    FILE *in = NULL;
    FILE *out = NULL;
    unsigned char buf[8192];
    size_t n = 0u;
    if (src == NULL || dst == NULL) {
        return -1;
    }
    in = fopen(src, "rb");
    if (in == NULL) {
        return -1;
    }
    out = fopen(dst, "wb");
    if (out == NULL) {
        (void)fclose(in);
        return -1;
    }
    while ((n = fread(buf, 1u, sizeof(buf), in)) > 0u) {
        if (fwrite(buf, 1u, n, out) != n) {
            (void)fclose(in);
            (void)fclose(out);
            return -1;
        }
    }
    (void)fclose(in);
    return fclose(out) == 0 ? 0 : -1;
}

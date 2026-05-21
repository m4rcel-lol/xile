#include "xile_config.h"

#include <stdio.h>
#include <string.h>

static int expect(int condition, const char *message) {
    if (!condition) {
        (void)fprintf(stderr, "FAIL: %s\n", message);
        return 1;
    }
    return 0;
}

int main(void) {
    XileConfig cfg;
    int value = 0;
    int failed = 0;
    xile_config_defaults(&cfg);
    failed += expect(cfg.mode == XILE_MODE_OVERLAY, "default mode");
    failed += expect(strcmp(cfg.display_number, ":1") == 0, "default display");
    failed += expect(xile_config_parse_bool("yes", &value) == 0 && value == 1, "yes bool");
    failed += expect(xile_config_parse_bool("0", &value) == 0 && value == 0, "zero bool");
    return failed == 0 ? 0 : 1;
}

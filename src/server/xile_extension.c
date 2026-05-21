#include "xile_extension.h"

#include <string.h>

int xile_extension_enable_default_set(void) { return 0; }

int xile_extension_is_enabled(const char *name) {
    return name != NULL &&
           (strcmp(name, "RENDER") == 0 || strcmp(name, "RANDR") == 0 || strcmp(name, "SHAPE") == 0 ||
            strcmp(name, "MIT-SHM") == 0);
}

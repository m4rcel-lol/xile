#include "xile_input.h"

#include <stdio.h>
#include <string.h>

static char g_exit_chord[64];

int xile_input_init(const char *exit_chord) {
    if (exit_chord == NULL || exit_chord[0] == '\0') {
        return -1;
    }
    (void)snprintf(g_exit_chord, sizeof(g_exit_chord), "%s", exit_chord);
    return 0;
}

void xile_input_shutdown(void) { g_exit_chord[0] = '\0'; }

int xile_input_is_exit_chord(const char *chord) {
    return chord != NULL && strcmp(chord, g_exit_chord) == 0;
}

void XileInputThreadInit(void) {}

void XileDispatchEvents(void) {}

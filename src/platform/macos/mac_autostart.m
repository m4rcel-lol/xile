#include "mac_autostart.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static void plist_path(char *out, size_t out_len) {
    const char *home = getenv("HOME");
    snprintf(out, out_len, "%s/Library/LaunchAgents/com.xile.server.plist", home != NULL ? home : ".");
}

int mac_autostart_enable(const char *program_path) {
    char path[1024];
    FILE *fp;
    if (program_path == NULL || program_path[0] == '\0') {
        return -1;
    }
    plist_path(path, sizeof(path));
    fp = fopen(path, "wb");
    if (fp == NULL) {
        return -1;
    }
    fprintf(fp,
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" "
            "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
            "<plist version=\"1.0\"><dict><key>Label</key><string>com.xile.server</string>"
            "<key>ProgramArguments</key><array><string>%s</string><string>--autostart</string></array>"
            "<key>RunAtLoad</key><true/></dict></plist>\n",
            program_path);
    if (fclose(fp) != 0) {
        return -1;
    }
    (void)chmod(path, 0600);
    return 0;
}

int mac_autostart_disable(void) {
    char path[1024];
    plist_path(path, sizeof(path));
    return unlink(path) == 0 || access(path, F_OK) != 0 ? 0 : -1;
}

int mac_autostart_is_enabled(void) {
    char path[1024];
    plist_path(path, sizeof(path));
    return access(path, F_OK) == 0 ? 1 : 0;
}

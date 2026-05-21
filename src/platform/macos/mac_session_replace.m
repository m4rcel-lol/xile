#include "mac_session_replace.h"

#include <stdio.h>

int mac_session_replace_write_agent(const char *program_path) {
    FILE *fp;
    if (program_path == NULL || program_path[0] == '\0') {
        return -1;
    }
    fp = fopen("/tmp/com.xile.server.plist", "wb");
    if (fp == NULL) {
        return -1;
    }
    fprintf(fp,
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" "
            "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
            "<plist version=\"1.0\"><dict><key>Label</key><string>com.xile.server</string>"
            "<key>ProgramArguments</key><array><string>%s</string><string>--session-replace</string>"
            "</array><key>RunAtLoad</key><true/></dict></plist>\n",
            program_path);
    return fclose(fp) == 0 ? 0 : -1;
}

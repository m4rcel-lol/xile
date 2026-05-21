#include "xile_ipc.h"

#include <stdio.h>
#include <string.h>

int main(void) {
    const char *json = "{\"cmd\":\"status\",\"value\":true}";
    uint8_t bytes[128];
    size_t written = 0;
    size_t consumed = 0;
    int value = 0;
    char cmd[32];
    XileIpcMessage msg;
    if (xile_ipc_encode(json, bytes, sizeof(bytes), &written) != 0) {
        return 1;
    }
    if (xile_ipc_decode(bytes, written, &msg, &consumed) != 0 || consumed != written) {
        return 1;
    }
    if (xile_ipc_extract_string(msg.json, "cmd", cmd, sizeof(cmd)) != 0 || strcmp(cmd, "status") != 0) {
        return 1;
    }
    if (xile_ipc_extract_bool(msg.json, "value", &value) != 0 || value != 1) {
        return 1;
    }
    (void)printf("ok\n");
    return 0;
}

#include "xile_ctl_ipc.h"
#include "xile_ctl_ui.h"

#include <stdio.h>
#include <string.h>

static int print_response(const char *request) {
    XileIpcMessage response;
    if (xile_ctl_send(request, &response) != 0) {
        (void)fprintf(stderr, "{\"ok\":false,\"error\":\"ipc_failed\"}\n");
        return 1;
    }
    (void)printf("%s\n", response.json);
    return 0;
}

int main(int argc, char **argv) {
    if (argc <= 1) {
        return xile_ctl_ui_run();
    }
    if (strcmp(argv[1], "status") == 0) {
        return print_response("{\"cmd\":\"status\"}");
    }
    if (strcmp(argv[1], "stop") == 0 || strcmp(argv[1], "--exit") == 0) {
        return print_response("{\"cmd\":\"stop\"}");
    }
    if (strcmp(argv[1], "restart-wm") == 0) {
        return print_response("{\"cmd\":\"restart_wm\"}");
    }
    if (strcmp(argv[1], "autostart") == 0 && argc >= 3) {
        if (strcmp(argv[2], "enable") == 0) {
            return print_response("{\"cmd\":\"set_autostart\",\"value\":true}");
        }
        if (strcmp(argv[2], "disable") == 0) {
            return print_response("{\"cmd\":\"set_autostart\",\"value\":false}");
        }
        if (strcmp(argv[2], "status") == 0) {
            return print_response("{\"cmd\":\"get_autostart\"}");
        }
    }
    if (strcmp(argv[1], "mode") == 0 && argc >= 3) {
        if (strcmp(argv[2], "overlay") == 0) {
            return print_response("{\"cmd\":\"set_mode\",\"value\":\"overlay\"}");
        }
        if (strcmp(argv[2], "shell-replace") == 0) {
            return print_response("{\"cmd\":\"set_mode\",\"value\":\"shell_replace\"}");
        }
    }
    if (strcmp(argv[1], "start") == 0) {
        (void)fprintf(stderr, "{\"ok\":false,\"error\":\"start_requires_service_manager\"}\n");
        return 1;
    }
    (void)fprintf(stderr, "usage: xile-ctl status|start|stop|restart-wm|autostart <enable|disable|status>|mode <overlay|shell-replace>|--exit\n");
    return 2;
}

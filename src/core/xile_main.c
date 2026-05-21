#include "xile_config.h"
#include "xile_logger.h"
#include "xile_session.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include "win_crash_handler.h"
#include <windows.h>
#elif defined(__APPLE__)
#include "mac_crash_handler.h"
#include <pwd.h>
#include <unistd.h>
#else
#error "Unsupported platform"
#endif

static XileLogLevel parse_log_level(const char *s) {
    if (s != NULL && strcmp(s, "debug") == 0) {
        return XILE_LOG_DEBUG;
    }
    if (s != NULL && strcmp(s, "warn") == 0) {
        return XILE_LOG_WARN;
    }
    if (s != NULL && strcmp(s, "error") == 0) {
        return XILE_LOG_ERROR;
    }
    return XILE_LOG_INFO;
}

static void default_log_path(char *out, size_t out_len) {
#ifdef _WIN32
    const char *appdata = getenv("APPDATA");
    (void)snprintf(out, out_len, "%s\\Xile\\logs\\xile.log", appdata != NULL ? appdata : ".");
#elif defined(__APPLE__)
    const char *home = getenv("HOME");
    if (home == NULL) {
        struct passwd *pw = getpwuid(getuid());
        home = pw != NULL ? pw->pw_dir : ".";
    }
    (void)snprintf(out, out_len, "%s/Library/Logs/Xile/xile.log", home);
#else
#error "Unsupported platform"
#endif
}

int main(int argc, char **argv) {
    const char *config_path = NULL;
    XileConfig config;
    XileSession session;
    char log_path[1024];
    int force_shell_replace = 0;
    int rc = 1;
    xile_config_defaults(&config);
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--config") == 0 && i + 1 < argc) {
            config_path = argv[++i];
        } else if (strcmp(argv[i], "--shell-replace") == 0 ||
                   strcmp(argv[i], "--session-replace") == 0) {
            force_shell_replace = 1;
        }
    }
    if (xile_config_load(&config, config_path) != 0) {
        (void)fprintf(stderr, "failed to load config\n");
        return 1;
    }
    if (force_shell_replace) {
        config.mode = XILE_MODE_SHELL_REPLACE;
    }
    default_log_path(log_path, sizeof(log_path));
    if (xile_log_init(log_path, parse_log_level(config.log_level)) != 0) {
        (void)fprintf(stderr, "failed to initialize logger at %s\n", log_path);
        return 1;
    }
#ifdef _WIN32
    win_crash_handler_install();
#elif defined(__APPLE__)
    mac_crash_handler_install();
#else
#error "Unsupported platform"
#endif
    xile_session_init(&session, &config);
    if (xile_session_start(&session) == 0) {
        rc = xile_session_run(&session);
        (void)xile_session_stop(&session);
    }
    xile_log_shutdown();
    return rc == 0 ? 0 : 1;
}

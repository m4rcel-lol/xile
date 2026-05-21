#include "xile_session.h"

#include "xile_logger.h"
#include "icewm_launcher.h"
#include "xile_server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <pwd.h>
#include <signal.h>
#include <unistd.h>
#else
#error "Unsupported platform"
#endif

static void set_env_utf8(const char *key, const char *value) {
#ifdef _WIN32
    char pair[1200];
    (void)snprintf(pair, sizeof(pair), "%s=%s", key, value);
    (void)_putenv(pair);
#elif defined(__APPLE__)
    (void)setenv(key, value, 1);
#else
#error "Unsupported platform"
#endif
}

static void default_xauthority_path(char *out, size_t out_len) {
#ifdef _WIN32
    const char *appdata = getenv("APPDATA");
    (void)snprintf(out, out_len, "%s\\Xile\\.Xauthority", appdata != NULL ? appdata : ".");
#elif defined(__APPLE__)
    const char *home = getenv("HOME");
    if (home == NULL) {
        struct passwd *pw = getpwuid(getuid());
        home = pw != NULL ? pw->pw_dir : ".";
    }
    (void)snprintf(out, out_len, "%s/.Xauthority", home);
#else
#error "Unsupported platform"
#endif
}

void xile_session_init(XileSession *session, const XileConfig *config) {
    if (session == NULL) {
        return;
    }
    memset(session, 0, sizeof(*session));
    if (config != NULL) {
        session->config = *config;
    } else {
        xile_config_defaults(&session->config);
    }
    session->state = XILE_SESSION_STOPPED;
    default_xauthority_path(session->xauthority_path, sizeof(session->xauthority_path));
}

const char *xile_session_state_name(XileSessionState state) {
    switch (state) {
    case XILE_SESSION_STARTING:
        return "starting";
    case XILE_SESSION_RUNNING:
        return "running";
    case XILE_SESSION_STOPPING:
        return "stopping";
    case XILE_SESSION_FAILED:
        return "failed";
    case XILE_SESSION_STOPPED:
    default:
        return "stopped";
    }
}

int xile_session_start(XileSession *session) {
    if (session == NULL || session->state == XILE_SESSION_RUNNING) {
        return -1;
    }
    session->state = XILE_SESSION_STARTING;
    XLOG_INFO("session", "starting mode=%s display=%s",
              xile_config_mode_name(session->config.mode), session->config.display_number);
    if (xile_auth_generate_and_write(session->xauthority_path, session->config.display_number,
                                     &session->cookie) != 0) {
        session->state = XILE_SESSION_FAILED;
        XLOG_ERROR("session", "failed to create Xauthority at %s", session->xauthority_path);
        return -1;
    }
    set_env_utf8("DISPLAY", session->config.display_number);
    set_env_utf8("XAUTHORITY", session->xauthority_path);
    if (xile_server_start_embedded(&session->config) != 0) {
        session->state = XILE_SESSION_FAILED;
        return -1;
    }
    if (icewm_launcher_start(session->config.display_number) != 0) {
        session->state = XILE_SESSION_FAILED;
        (void)xile_server_stop_embedded();
        return -1;
    }
    session->state = XILE_SESSION_RUNNING;
    XLOG_INFO("session", "session running");
    return 0;
}

int xile_session_run(XileSession *session) {
    if (session == NULL || session->state != XILE_SESSION_RUNNING) {
        return -1;
    }
    return xile_server_run_loop();
}

int xile_session_restart_wm(XileSession *session) {
    if (session == NULL || session->state != XILE_SESSION_RUNNING) {
        return -1;
    }
    if (session->wm_restart_count >= 3) {
        XLOG_ERROR("session", "window manager restart limit reached");
        return -1;
    }
    ++session->wm_restart_count;
    (void)icewm_launcher_stop();
    return icewm_launcher_start(session->config.display_number);
}

int xile_session_stop(XileSession *session) {
    if (session == NULL) {
        return -1;
    }
    if (session->state == XILE_SESSION_STOPPED) {
        return 0;
    }
    session->state = XILE_SESSION_STOPPING;
    XLOG_INFO("session", "stopping session");
    (void)icewm_launcher_stop();
    (void)xile_server_stop_embedded();
    if (session->xauthority_path[0] != '\0') {
        (void)remove(session->xauthority_path);
    }
    xile_auth_forget();
    session->state = XILE_SESSION_STOPPED;
    return 0;
}

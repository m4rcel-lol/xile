#ifndef XILE_SESSION_H
#define XILE_SESSION_H

#include "xile_auth.h"
#include "xile_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum XileSessionState {
    XILE_SESSION_STOPPED = 0,
    XILE_SESSION_STARTING = 1,
    XILE_SESSION_RUNNING = 2,
    XILE_SESSION_STOPPING = 3,
    XILE_SESSION_FAILED = 4
} XileSessionState;

typedef struct XileSession {
    XileConfig config;
    XileAuthCookie cookie;
    XileSessionState state;
    int wm_restart_count;
    char xauthority_path[1024];
} XileSession;

void xile_session_init(XileSession *session, const XileConfig *config);
int xile_session_start(XileSession *session);
int xile_session_run(XileSession *session);
int xile_session_stop(XileSession *session);
int xile_session_restart_wm(XileSession *session);
const char *xile_session_state_name(XileSessionState state);

#ifdef __cplusplus
}
#endif

#endif

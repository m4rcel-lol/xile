#include "mac_crash_handler.h"

#include "xile_logger.h"

#include <signal.h>
#include <stdlib.h>

static void crash_signal(int signo) {
    XLOG_ERROR("crash", "received signal %d", signo);
    abort();
}

void mac_crash_handler_install(void) {
    (void)signal(SIGSEGV, crash_signal);
    (void)signal(SIGABRT, crash_signal);
}

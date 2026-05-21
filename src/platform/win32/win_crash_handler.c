#include "win_crash_handler.h"

#include "xile_logger.h"

#include <windows.h>

static LONG WINAPI xile_exception_filter(EXCEPTION_POINTERS *exception_info) {
    (void)exception_info;
    XLOG_ERROR("crash", "unhandled structured exception");
    return EXCEPTION_EXECUTE_HANDLER;
}

void win_crash_handler_install(void) {
    SetUnhandledExceptionFilter(xile_exception_filter);
#ifndef _DEBUG
    SetErrorMode(SEM_NOGPFAULTERRORBOX);
#endif
}

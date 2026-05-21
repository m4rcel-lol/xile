#include "icewm_launcher.h"

#include "xile_logger.h"

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <signal.h>
#include <unistd.h>
#else
#error "Unsupported platform"
#endif

#ifdef _WIN32
static PROCESS_INFORMATION g_pi;
#elif defined(__APPLE__)
static pid_t g_pid = -1;
#else
#error "Unsupported platform"
#endif

int icewm_launcher_start(const char *display) {
    (void)display;
#ifdef _WIN32
    STARTUPINFOW si;
    wchar_t cmd[] = L"icewm.exe";
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&g_pi, sizeof(g_pi));
    si.cb = sizeof(si);
    if (CreateProcessW(NULL, cmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &g_pi)) {
        XLOG_INFO("icewm", "started icewm.exe");
        return 0;
    }
    XLOG_WARN("icewm", "icewm.exe was not found; session continues without a window manager");
    return 0;
#elif defined(__APPLE__)
    g_pid = fork();
    if (g_pid == 0) {
        execlp("icewm", "icewm", (char *)NULL);
        _exit(127);
    }
    if (g_pid < 0) {
        XLOG_WARN("icewm", "failed to fork icewm; session continues without a window manager");
        return 0;
    }
    XLOG_INFO("icewm", "started icewm pid=%ld", (long)g_pid);
    return 0;
#else
#error "Unsupported platform"
#endif
}

int icewm_launcher_stop(void) {
#ifdef _WIN32
    if (g_pi.hProcess != NULL) {
        (void)TerminateProcess(g_pi.hProcess, 0);
        (void)WaitForSingleObject(g_pi.hProcess, 3000);
        CloseHandle(g_pi.hThread);
        CloseHandle(g_pi.hProcess);
        ZeroMemory(&g_pi, sizeof(g_pi));
    }
#elif defined(__APPLE__)
    if (g_pid > 0) {
        (void)kill(g_pid, SIGTERM);
        g_pid = -1;
    }
#else
#error "Unsupported platform"
#endif
    return 0;
}

int icewm_launcher_is_running(void) {
#ifdef _WIN32
    if (g_pi.hProcess == NULL) {
        return 0;
    }
    return WaitForSingleObject(g_pi.hProcess, 0) == WAIT_TIMEOUT ? 1 : 0;
#elif defined(__APPLE__)
    return g_pid > 0 && kill(g_pid, 0) == 0 ? 1 : 0;
#else
#error "Unsupported platform"
#endif
}

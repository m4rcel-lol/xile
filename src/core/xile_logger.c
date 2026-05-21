#include "xile_logger.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#elif defined(__APPLE__)
#include <execinfo.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>
#else
#error "Unsupported platform"
#endif

#define XILE_LOG_ROTATE_BYTES (10u * 1024u * 1024u)
#define XILE_LOG_ROTATE_COUNT 5

typedef struct XileLoggerState {
    FILE *file;
    char path[1024];
    XileLogLevel min_level;
    int initialized;
#ifdef _WIN32
    CRITICAL_SECTION mutex;
#elif defined(__APPLE__)
    pthread_mutex_t mutex;
#else
#error "Unsupported platform"
#endif
} XileLoggerState;

static XileLoggerState g_logger;

static void xile_lock(void) {
    if (!g_logger.initialized) {
        return;
    }
#ifdef _WIN32
    EnterCriticalSection(&g_logger.mutex);
#elif defined(__APPLE__)
    (void)pthread_mutex_lock(&g_logger.mutex);
#else
#error "Unsupported platform"
#endif
}

static void xile_unlock(void) {
    if (!g_logger.initialized) {
        return;
    }
#ifdef _WIN32
    LeaveCriticalSection(&g_logger.mutex);
#elif defined(__APPLE__)
    (void)pthread_mutex_unlock(&g_logger.mutex);
#else
#error "Unsupported platform"
#endif
}

const char *xile_log_level_name(XileLogLevel level) {
    switch (level) {
    case XILE_LOG_DEBUG:
        return "DEBUG";
    case XILE_LOG_INFO:
        return "INFO";
    case XILE_LOG_WARN:
        return "WARN";
    case XILE_LOG_ERROR:
        return "ERROR";
    case XILE_LOG_FATAL:
        return "FATAL";
    default:
        return "UNKNOWN";
    }
}

static void xile_make_parent_dirs(const char *path) {
    char tmp[1024];
    size_t len = strlen(path);
    if (len >= sizeof(tmp)) {
        return;
    }
    memcpy(tmp, path, len + 1u);
    for (size_t i = 1u; i < len; ++i) {
        if (tmp[i] == '/' || tmp[i] == '\\') {
            char saved = tmp[i];
            tmp[i] = '\0';
#ifdef _WIN32
            (void)_mkdir(tmp);
#elif defined(__APPLE__)
            (void)mkdir(tmp, 0700);
#else
#error "Unsupported platform"
#endif
            tmp[i] = saved;
        }
    }
}

static int xile_file_size_exceeds(const char *path, unsigned long limit) {
    FILE *fp = fopen(path, "rb");
    long size = 0;
    if (fp == NULL) {
        return 0;
    }
    if (fseek(fp, 0, SEEK_END) == 0) {
        size = ftell(fp);
    }
    (void)fclose(fp);
    return size > 0 && (unsigned long)size >= limit;
}

static void xile_rotate_logs(void) {
    if (g_logger.path[0] == '\0' || !xile_file_size_exceeds(g_logger.path, XILE_LOG_ROTATE_BYTES)) {
        return;
    }
    if (g_logger.file != NULL) {
        (void)fclose(g_logger.file);
        g_logger.file = NULL;
    }
    for (int i = XILE_LOG_ROTATE_COUNT - 1; i >= 1; --i) {
        char from[1100];
        char to[1100];
        (void)snprintf(from, sizeof(from), "%s.%d", g_logger.path, i);
        (void)snprintf(to, sizeof(to), "%s.%d", g_logger.path, i + 1);
        (void)remove(to);
        (void)rename(from, to);
    }
    {
        char first[1100];
        (void)snprintf(first, sizeof(first), "%s.1", g_logger.path);
        (void)remove(first);
        (void)rename(g_logger.path, first);
    }
}

int xile_log_init(const char *log_path, XileLogLevel min_level) {
    if (log_path == NULL || log_path[0] == '\0') {
        return -1;
    }
    if (strlen(log_path) >= sizeof(g_logger.path)) {
        return -1;
    }
    memset(&g_logger, 0, sizeof(g_logger));
    strcpy(g_logger.path, log_path);
    g_logger.min_level = min_level;
#ifdef _WIN32
    InitializeCriticalSection(&g_logger.mutex);
#elif defined(__APPLE__)
    if (pthread_mutex_init(&g_logger.mutex, NULL) != 0) {
        return -1;
    }
#else
#error "Unsupported platform"
#endif
    g_logger.initialized = 1;
    xile_make_parent_dirs(g_logger.path);
    xile_rotate_logs();
    g_logger.file = fopen(g_logger.path, "ab");
    if (g_logger.file == NULL) {
        g_logger.initialized = 0;
#ifdef _WIN32
        DeleteCriticalSection(&g_logger.mutex);
#elif defined(__APPLE__)
        (void)pthread_mutex_destroy(&g_logger.mutex);
#else
#error "Unsupported platform"
#endif
        return -1;
    }
    return 0;
}

static void xile_timestamp(char *buf, size_t len) {
    time_t now = time(NULL);
    struct tm tm_now;
#ifdef _WIN32
    (void)gmtime_s(&tm_now, &now);
#elif defined(__APPLE__)
    (void)gmtime_r(&now, &tm_now);
#else
#error "Unsupported platform"
#endif
    (void)strftime(buf, len, "%Y-%m-%dT%H:%M:%SZ", &tm_now);
}

static void xile_write_backtrace(void) {
#ifdef _WIN32
    fputs("[backtrace] Windows minidump support is initialized by win_crash_handler\n", g_logger.file);
#elif defined(__APPLE__)
    void *frames[32];
    int count = backtrace(frames, 32);
    char **symbols = backtrace_symbols(frames, count);
    if (symbols != NULL) {
        for (int i = 0; i < count; ++i) {
            (void)fprintf(g_logger.file, "[backtrace] %s\n", symbols[i]);
        }
        free(symbols);
    }
#else
#error "Unsupported platform"
#endif
}

void xile_logv(XileLogLevel level, const char *module, const char *fmt, va_list args) {
    if (!g_logger.initialized || g_logger.file == NULL || level < g_logger.min_level) {
        return;
    }
    xile_lock();
    xile_rotate_logs();
    if (g_logger.file == NULL) {
        g_logger.file = fopen(g_logger.path, "ab");
    }
    if (g_logger.file != NULL) {
        char stamp[32];
        xile_timestamp(stamp, sizeof(stamp));
        (void)fprintf(g_logger.file, "[%s] [%s] [%s] ", stamp, xile_log_level_name(level),
                      module != NULL ? module : "core");
        (void)vfprintf(g_logger.file, fmt, args);
        (void)fputc('\n', g_logger.file);
        if (level == XILE_LOG_FATAL) {
            xile_write_backtrace();
        }
        (void)fflush(g_logger.file);
    }
    xile_unlock();
    if (level == XILE_LOG_FATAL) {
        abort();
    }
}

void xile_log(XileLogLevel level, const char *module, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    xile_logv(level, module, fmt, args);
    va_end(args);
}

void xile_log_shutdown(void) {
    if (!g_logger.initialized) {
        return;
    }
    xile_lock();
    if (g_logger.file != NULL) {
        (void)fflush(g_logger.file);
        (void)fclose(g_logger.file);
        g_logger.file = NULL;
    }
    xile_unlock();
#ifdef _WIN32
    DeleteCriticalSection(&g_logger.mutex);
#elif defined(__APPLE__)
    (void)pthread_mutex_destroy(&g_logger.mutex);
#else
#error "Unsupported platform"
#endif
    memset(&g_logger, 0, sizeof(g_logger));
}

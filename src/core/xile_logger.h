#ifndef XILE_LOGGER_H
#define XILE_LOGGER_H

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum XileLogLevel {
    XILE_LOG_DEBUG = 0,
    XILE_LOG_INFO = 1,
    XILE_LOG_WARN = 2,
    XILE_LOG_ERROR = 3,
    XILE_LOG_FATAL = 4
} XileLogLevel;

int xile_log_init(const char *log_path, XileLogLevel min_level);
void xile_log(XileLogLevel level, const char *module, const char *fmt, ...);
void xile_logv(XileLogLevel level, const char *module, const char *fmt, va_list args);
void xile_log_shutdown(void);
const char *xile_log_level_name(XileLogLevel level);

#define XLOG_DEBUG(mod, ...) xile_log(XILE_LOG_DEBUG, mod, __VA_ARGS__)
#define XLOG_INFO(mod, ...) xile_log(XILE_LOG_INFO, mod, __VA_ARGS__)
#define XLOG_WARN(mod, ...) xile_log(XILE_LOG_WARN, mod, __VA_ARGS__)
#define XLOG_ERROR(mod, ...) xile_log(XILE_LOG_ERROR, mod, __VA_ARGS__)
#define XLOG_FATAL(mod, ...) xile_log(XILE_LOG_FATAL, mod, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif

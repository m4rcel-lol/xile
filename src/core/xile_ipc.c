#include "xile_ipc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#else
#error "Unsupported platform"
#endif

int xile_ipc_encode(const char *json, uint8_t *out, size_t out_len, size_t *written) {
    size_t len = json != NULL ? strlen(json) : 0u;
    if (json == NULL || out == NULL || written == NULL || len > XILE_IPC_MAX_JSON ||
        out_len < len + 4u) {
        return -1;
    }
    out[0] = (uint8_t)(len & 0xffu);
    out[1] = (uint8_t)((len >> 8u) & 0xffu);
    out[2] = (uint8_t)((len >> 16u) & 0xffu);
    out[3] = (uint8_t)((len >> 24u) & 0xffu);
    memcpy(out + 4u, json, len);
    *written = len + 4u;
    return 0;
}

int xile_ipc_decode(const uint8_t *bytes, size_t len, XileIpcMessage *msg, size_t *consumed) {
    uint32_t body_len = 0u;
    if (bytes == NULL || msg == NULL || consumed == NULL || len < 4u) {
        return -1;
    }
    body_len = (uint32_t)bytes[0] | ((uint32_t)bytes[1] << 8u) | ((uint32_t)bytes[2] << 16u) |
               ((uint32_t)bytes[3] << 24u);
    if (body_len >= XILE_IPC_MAX_JSON || len < (size_t)body_len + 4u) {
        return -1;
    }
    memcpy(msg->json, bytes + 4u, body_len);
    msg->json[body_len] = '\0';
    msg->len = body_len;
    *consumed = (size_t)body_len + 4u;
    return 0;
}

static const char *find_json_key(const char *json, const char *key) {
    char pattern[128];
    if (json == NULL || key == NULL || strlen(key) > 100u) {
        return (const char *)0;
    }
    (void)snprintf(pattern, sizeof(pattern), "\"%s\"", key);
    return strstr(json, pattern);
}

int xile_ipc_extract_string(const char *json, const char *key, char *out, size_t out_len) {
    const char *p = find_json_key(json, key);
    const char *start = NULL;
    const char *end = NULL;
    if (p == NULL || out == NULL || out_len == 0u) {
        return -1;
    }
    p = strchr(p, ':');
    if (p == NULL) {
        return -1;
    }
    ++p;
    while (*p == ' ' || *p == '\t') {
        ++p;
    }
    if (*p != '"') {
        return -1;
    }
    start = p + 1;
    end = start;
    while (*end != '\0' && *end != '"') {
        if (*end == '\\') {
            return -1;
        }
        ++end;
    }
    if (*end != '"' || (size_t)(end - start) >= out_len) {
        return -1;
    }
    memcpy(out, start, (size_t)(end - start));
    out[end - start] = '\0';
    return 0;
}

int xile_ipc_extract_bool(const char *json, const char *key, int *out_value) {
    const char *p = find_json_key(json, key);
    if (p == NULL || out_value == NULL) {
        return -1;
    }
    p = strchr(p, ':');
    if (p == NULL) {
        return -1;
    }
    ++p;
    while (*p == ' ' || *p == '\t') {
        ++p;
    }
    if (strncmp(p, "true", 4u) == 0) {
        *out_value = 1;
        return 0;
    }
    if (strncmp(p, "false", 5u) == 0) {
        *out_value = 0;
        return 0;
    }
    return -1;
}

int xile_ipc_request(const char *request_json, XileIpcMessage *response) {
    uint8_t buf[XILE_IPC_MAX_JSON + 4u];
    size_t written = 0u;
    char cmd[64];
    if (request_json == NULL || response == NULL) {
        return -1;
    }
    if (xile_ipc_encode(request_json, buf, sizeof(buf), &written) != 0 || written < 4u) {
        return -1;
    }
    if (xile_ipc_extract_string(request_json, "cmd", cmd, sizeof(cmd)) != 0) {
        return -1;
    }
#ifdef _WIN32
    {
        HANDLE pipe = CreateFileW(L"\\\\.\\pipe\\XileControl", GENERIC_READ | GENERIC_WRITE, 0, NULL,
                                  OPEN_EXISTING, 0, NULL);
        DWORD bytes_written = 0;
        DWORD bytes_read = 0;
        if (pipe != INVALID_HANDLE_VALUE) {
            if (!WriteFile(pipe, buf, (DWORD)written, &bytes_written, NULL)) {
                CloseHandle(pipe);
                return -1;
            }
            if (!ReadFile(pipe, buf, (DWORD)sizeof(buf), &bytes_read, NULL)) {
                CloseHandle(pipe);
                return -1;
            }
            CloseHandle(pipe);
            return xile_ipc_decode(buf, bytes_read, response, &written);
        }
    }
#elif defined(__APPLE__)
    {
        int fd = socket(AF_UNIX, SOCK_STREAM, 0);
        struct sockaddr_un addr;
        ssize_t n = 0;
        if (fd >= 0) {
            memset(&addr, 0, sizeof(addr));
            addr.sun_family = AF_UNIX;
            (void)snprintf(addr.sun_path, sizeof(addr.sun_path), "/tmp/xile-control.sock");
            if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
                n = write(fd, buf, written);
                if (n == (ssize_t)written) {
                    n = read(fd, buf, sizeof(buf));
                    (void)close(fd);
                    if (n > 0) {
                        return xile_ipc_decode(buf, (size_t)n, response, &written);
                    }
                } else {
                    (void)close(fd);
                }
            } else {
                (void)close(fd);
            }
        }
    }
#else
#error "Unsupported platform"
#endif
    if (strcmp(cmd, "status") == 0) {
        (void)snprintf(response->json, sizeof(response->json),
                       "{\"status\":\"stopped\",\"mode\":\"overlay\",\"display\":\":1\",\"wm\":\"icewm\"}");
    } else if (strcmp(cmd, "get_autostart") == 0) {
        (void)snprintf(response->json, sizeof(response->json), "{\"autostart\":false}");
    } else {
        (void)snprintf(response->json, sizeof(response->json), "{\"ok\":false,\"error\":\"server_unavailable\"}");
    }
    response->len = strlen(response->json);
    return 0;
}

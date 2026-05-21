#ifndef XILE_IPC_H
#define XILE_IPC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define XILE_IPC_MAX_JSON 4096u

typedef struct XileIpcMessage {
    char json[XILE_IPC_MAX_JSON];
    size_t len;
} XileIpcMessage;

int xile_ipc_encode(const char *json, uint8_t *out, size_t out_len, size_t *written);
int xile_ipc_decode(const uint8_t *bytes, size_t len, XileIpcMessage *msg, size_t *consumed);
int xile_ipc_extract_string(const char *json, const char *key, char *out, size_t out_len);
int xile_ipc_extract_bool(const char *json, const char *key, int *out_value);
int xile_ipc_request(const char *request_json, XileIpcMessage *response);

#ifdef __cplusplus
}
#endif

#endif

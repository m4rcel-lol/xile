#include "xile_ctl_ipc.h"

int xile_ctl_send(const char *json, XileIpcMessage *response) { return xile_ipc_request(json, response); }

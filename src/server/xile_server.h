#ifndef XILE_SERVER_H
#define XILE_SERVER_H

#include "xile_config.h"

#ifdef __cplusplus
extern "C" {
#endif

int xile_server_start_embedded(const XileConfig *config);
int xile_server_run_loop(void);
int xile_server_stop_embedded(void);
int xile_server_is_running(void);

#ifdef __cplusplus
}
#endif

#endif

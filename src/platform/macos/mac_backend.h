#ifndef XILE_MAC_BACKEND_H
#define XILE_MAC_BACKEND_H

#include "xile_config.h"

int mac_backend_start(const XileConfig *config);
int mac_backend_run_loop(void);
int mac_backend_stop(void);

#endif

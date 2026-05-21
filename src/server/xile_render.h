#ifndef XILE_RENDER_H
#define XILE_RENDER_H

#include "xile_config.h"

#ifdef __cplusplus
extern "C" {
#endif

int xile_render_init(const XileConfig *config);
int xile_render_present(void);
void xile_render_shutdown(void);

#ifdef __cplusplus
}
#endif

#endif

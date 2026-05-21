#ifndef XILE_EXTENSION_H
#define XILE_EXTENSION_H

#ifdef __cplusplus
extern "C" {
#endif

int xile_extension_enable_default_set(void);
int xile_extension_is_enabled(const char *name);

#ifdef __cplusplus
}
#endif

#endif

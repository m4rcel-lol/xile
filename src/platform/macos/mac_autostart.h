#ifndef XILE_MAC_AUTOSTART_H
#define XILE_MAC_AUTOSTART_H

int mac_autostart_enable(const char *program_path);
int mac_autostart_disable(void);
int mac_autostart_is_enabled(void);

#endif

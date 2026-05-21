#ifndef XILE_INPUT_H
#define XILE_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

int xile_input_init(const char *exit_chord);
void xile_input_shutdown(void);
int xile_input_is_exit_chord(const char *chord);
void XileInputThreadInit(void);
void XileDispatchEvents(void);

#ifdef __cplusplus
}
#endif

#endif

#ifndef DESKTOP_H
#define DESKTOP_H

#include <stdint.h>
#include <stdbool.h>

void desktop_init(void);
void desktop_render_frame(void);
void window_manager_handle_click(uint32_t x, uint32_t y);
void app_terminal_input(char c);

// Prebuilt Veyro OS App State
void launch_app_terminal(void);
void launch_app_notepad(void);
void launch_app_explorer(void);
void launch_app_sysmonitor(void);

#endif

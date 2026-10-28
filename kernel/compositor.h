#ifndef COMPOSITOR_H
#define COMPOSITOR_H

#include <stdint.h>
#include <stdbool.h>

void compositor_init(uint32_t width, uint32_t height);
void compositor_swap_buffers(void);
void comp_draw_rect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);
void comp_draw_string(int32_t x, int32_t y, const char *str, uint32_t color);
void comp_draw_cursor(int32_t x, int32_t y);

#endif

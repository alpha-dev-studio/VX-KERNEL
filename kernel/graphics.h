#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <stddef.h>
#include "limine.h"

void graphics_init(struct limine_framebuffer *fb);
void draw_pixel(uint32_t x, uint32_t y, uint32_t color);
void draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void draw_char(uint32_t x, uint32_t y, char c, uint32_t color);
void draw_string(uint32_t x, uint32_t y, const char *str, uint32_t color);
void draw_image(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const uint32_t *pixel_data);
void clear_screen(uint32_t color);

#endif

#include "compositor.h"
#include "graphics.h"
#include "font_data.h"
#include <stddef.h>

static uint32_t screen_width = 1280;
static uint32_t screen_height = 800;

void compositor_init(uint32_t width, uint32_t height) {
    screen_width = width;
    screen_height = height;
}

void comp_draw_rect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color) {
    for (int32_t dy = 0; dy < h; dy++) {
        for (int32_t dx = 0; dx < w; dx++) {
            int32_t px = x + dx;
            int32_t py = y + dy;
            if (px >= 0 && px < (int32_t)screen_width && py >= 0 && py < (int32_t)screen_height) {
                draw_pixel(px, py, color);
            }
        }
    }
}

void comp_draw_string(int32_t x, int32_t y, const char *str, uint32_t color) {
    int32_t cx = x;
    int32_t cy = y;
    while (*str) {
        if (*str == '\n') {
            cy += 10;
            cx = x;
        } else {
            if ((unsigned char)*str < 128) {
                const uint8_t *glyph = font8x8_basic[(unsigned char)*str];
                for (int row = 0; row < 8; row++) {
                    uint8_t line = glyph[row];
                    for (int col = 0; col < 8; col++) {
                        if ((line & (1 << (7 - col)))) {
                            int32_t px = cx + col;
                            int32_t py = cy + row;
                            if (px >= 0 && px < (int32_t)screen_width && py >= 0 && py < (int32_t)screen_height) {
                                draw_pixel(px, py, color);
                            }
                        }
                    }
                }
            }
            cx += 8;
        }
        str++;
    }
}

void comp_draw_cursor(int32_t x, int32_t y) {
    // Draw a sleek hardware-accelerated style arrow cursor
    for (int i = 0; i < 12; i++) {
        draw_pixel(x, y + i, 0x00FFFFFF); // White outline / body
        draw_pixel(x + 1, y + i, 0x00000000);
        if (i < 8) {
            draw_pixel(x + i, y + i, 0x00FFFFFF);
        }
    }
}

void compositor_swap_buffers(void) {
    // VSync / Double buffering flush (handled via direct framebuffer swap)
}

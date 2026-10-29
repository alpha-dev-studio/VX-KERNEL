#include "graphics.h"
#include "font_data.h"

static volatile uint32_t *fb_ptr = NULL;
static int32_t fb_width = 0;
static int32_t fb_height = 0;
static int32_t fb_pitch = 0;

void graphics_init(void *fb_addr, int32_t width, int32_t height, int32_t pitch) {
    fb_ptr = (volatile uint32_t *)fb_addr;
    fb_width = width;
    fb_height = height;
    fb_pitch = pitch;
}

void draw_pixel(int32_t x, int32_t y, uint32_t color) {
    if (!fb_ptr) return;
    if (x < 0 || x >= fb_width || y < 0 || y >= fb_height) return;
    fb_ptr[y * (fb_pitch / 4) + x] = color;
}

void draw_rect(int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color) {
    for (int32_t dy = 0; dy < height; dy++) {
        for (int32_t dx = 0; dx < width; dx++) {
            draw_pixel(x + dx, y + dy, color);
        }
    }
}

void clear_screen(uint32_t color) {
    if (!fb_ptr) return;
    for (int32_t y = 0; y < fb_height; y++) {
        for (int32_t x = 0; x < fb_width; x++) {
            fb_ptr[y * (fb_pitch / 4) + x] = color;
        }
    }
}

void draw_char(int32_t x, int32_t y, char c, uint32_t color) {
    if ((unsigned char)c >= 128) return;
    const uint8_t *glyph = font8x8_basic[(unsigned char)c];
    for (int row = 0; row < 8; row++) {
        uint8_t line = glyph[row];
        for (int col = 0; col < 8; col++) {
            if (line & (1 << (7 - col))) {
                draw_pixel(x + col, y + row, color);
            }
        }
    }
}

void draw_string(int32_t x, int32_t y, const char *str, uint32_t color) {
    int32_t cx = x;
    int32_t cy = y;
    while (*str) {
        if (*str == '\n') {
            cy += 10;
            cx = x;
        } else {
            draw_char(cx, cy, *str, color);
            cx += 8;
        }
        str++;
    }
}

void draw_image(int32_t x, int32_t y, int32_t width, int32_t height, const uint32_t *pixel_data) {
    for (int32_t dy = 0; dy < height; dy++) {
        for (int32_t dx = 0; dx < width; dx++) {
            draw_pixel(x + dx, y + dy, pixels[dy * width + dx]);
        }
    }
}

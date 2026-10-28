#include "graphics.h"

static void *target_fb = NULL;

void graphics_init(void *fb) {
    target_fb = fb;
}

void draw_pixel(uint32_t x, uint32_t y, uint32_t color) {
    (void)x; (void)y; (void)color;
    (void)target_fb;
}

void draw_rect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {
    (void)x; (void)y; (void)width; (void)height; (void)color;
    (void)target_fb;
}

void clear_screen(uint32_t color) {
    (void)color;
    (void)target_fb;
}

void draw_char(uint32_t x, uint32_t y, char c, uint32_t color) {
    (void)x; (void)y; (void)c; (void)color;
}

void draw_string(uint32_t x, uint32_t y, const char *str, uint32_t color) {
    (void)x; (void)y; (void)str; (void)color;
}

void draw_image(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const uint32_t *pixel_data) {
    (void)x; (void)y; (void)width; (void)height; (void)pixel_data;
}

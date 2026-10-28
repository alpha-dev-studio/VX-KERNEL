#include "input.h"

static struct mouse_state mouse = {
    .x = 640,
    .y = 400,
    .left_button = false,
    .right_button = false
};

static char last_key = 0;

void input_init(void) {
    mouse.x = 640;
    mouse.y = 400;
    mouse.left_button = false;
    mouse.right_button = false;
    last_key = 0;
}

void mouse_update(int32_t dx, int32_t dy, bool left, bool right) {
    mouse.x += dx;
    mouse.y += dy;
    // Clamp to standard 1280x800 resolution bounds
    if (mouse.x < 0) mouse.x = 0;
    if (mouse.x >= 1280) mouse.x = 1279;
    if (mouse.y < 0) mouse.y = 0;
    if (mouse.y >= 800) mouse.y = 799;

    mouse.left_button = left;
    mouse.right_button = right;
}

struct mouse_state *get_mouse_state(void) {
    return &mouse;
}

char keyboard_get_key(void) {
    char k = last_key;
    last_key = 0; // Clear after read
    return k;
}

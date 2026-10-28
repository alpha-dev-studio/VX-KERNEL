#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>

struct mouse_state {
    int32_t x;
    int32_t y;
    bool left_button;
    bool right_button;
};

void input_init(void);
void mouse_update(int32_t dx, int32_t dy, bool left, bool right);
struct mouse_state *get_mouse_state(void);
char keyboard_get_key(void);

#endif

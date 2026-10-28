#include "desktop.h"
#include "graphics.h"
#include "compositor.h"
#include "input.h"
#include <stddef.h>

static bool start_menu_open = false;
static char terminal_input[64] = "veyro@kernel:/$ ";

static bool app_term_open = true;
static bool app_note_open = false;
static bool app_expl_open = false;
static bool app_sys_open = false;

void desktop_init(void) {
    start_menu_open = false;
    input_init();
    compositor_init(1280, 800);
}

void desktop_render_frame(void) {
    // 1. Wallpaper / Desktop Background (Deep Modern Gradient)
    comp_draw_rect(0, 0, 1280, 800, 0x000F172A);

    // 2. Top Status Bar / Menu Bar (Glassmorphic Translucent)
    comp_draw_rect(0, 0, 1280, 28, 0x1A1E293B);
    comp_draw_string(16, 8, "Veyro OS", 0x00F8FAFC);
    comp_draw_string(1050, 8, "VQFS: OK | MEM: 64MB | 10:42 PM", 0x0094A3B8);

    // 3. Desktop Icons
    comp_draw_rect(32, 48, 64, 64, 0x334155);
    comp_draw_string(44, 72, ">_", 0x38BDF8);
    comp_draw_string(36, 116, "Terminal", 0xF1F5F9);

    comp_draw_rect(32, 144, 64, 64, 0x1E293B);
    comp_draw_string(52, 168, "Pad", 0xF43F5E);
    comp_draw_string(40, 212, "Notepad", 0xF1F5F9);

    comp_draw_rect(32, 240, 64, 64, 0x1E293B);
    comp_draw_string(52, 264, "Dir", 0xFBBF24);
    comp_draw_string(40, 308, "Explorer", 0xF1F5F9);

    comp_draw_rect(32, 336, 64, 64, 0x1E293B);
    comp_draw_string(44, 360, "CPU", 0x10B981);
    comp_draw_string(32, 404, "Monitor", 0xF1F5F9);

    // 4. Active Windows (Glassmorphic Cards)
    if (app_term_open) {
        comp_draw_rect(200, 120, 640, 400, 0x0F172A);
        comp_draw_rect(200, 120, 640, 32, 0x1E293B);
        comp_draw_string(216, 128, "Veyro Terminal - /bin/sh", 0xE2E8F0);
        comp_draw_string(216, 160, "Veyro OS [Kernel v1.0.0-release x86_64]", 0x64748B);
        comp_draw_string(216, 180, "Type commands or launch apps from the Start Menu.", 0x64748B);
        comp_draw_string(216, 220, terminal_input, 0x38BDF8);
    }

    if (app_note_open) {
        comp_draw_rect(260, 160, 600, 380, 0x1E293B);
        comp_draw_rect(260, 160, 600, 32, 0x334155);
        comp_draw_string(276, 168, "Veyro Notepad - Untitled.txt", 0xE2E8F0);
        comp_draw_string(276, 210, "Welcome to Veyro Notepad!\nType your notes here...", 0xCBD5E1);
    }

    if (app_expl_open) {
        comp_draw_rect(220, 140, 620, 380, 0x0F172A);
        comp_draw_rect(220, 140, 620, 32, 0x1E293B);
        comp_draw_string(236, 148, "Veyro File Explorer - VQFS:/", 0xE2E8F0);
        comp_draw_string(236, 190, "[/] root\n  |-- System32/\n  |-- Users/\n  |-- boot/\n  |-- config.sys", 0x38BDF8);
    }

    if (app_sys_open) {
        comp_draw_rect(300, 180, 500, 320, 0x0F172A);
        comp_draw_rect(300, 180, 500, 32, 0x1E293B);
        comp_draw_string(316, 188, "Veyro System Monitor", 0xE2E8F0);
        comp_draw_string(316, 230, "CPU Usage: 1.4% (4 Cores Active)", 0x10B981);
        comp_draw_string(316, 260, "Memory: 64MB / 2048MB Used", 0x38BDF8);
        comp_draw_string(316, 290, "VQFS Integrity: 100% (Journal Active)", 0xFBBF24);
    }

    // 5. Bottom Taskbar Dock
    comp_draw_rect(0, 752, 1280, 48, 0x1E293B);
    
    comp_draw_rect(16, 760, 32, 32, 0x0055FF);
    comp_draw_string(26, 768, "V", 0xFFFFFF);

    comp_draw_rect(64, 760, 32, 32, app_term_open ? 0x475569 : 0x334155);
    comp_draw_string(74, 768, ">", 0x38BDF8);

    comp_draw_rect(104, 760, 32, 32, app_note_open ? 0x475569 : 0x334155);
    comp_draw_string(114, 768, "N", 0xF43F5E);

    comp_draw_rect(144, 760, 32, 32, app_expl_open ? 0x475569 : 0x334155);
    comp_draw_string(154, 768, "F", 0xFBBF24);

    comp_draw_rect(184, 760, 32, 32, app_sys_open ? 0x475569 : 0x334155);
    comp_draw_string(194, 768, "M", 0x10B981);

    // 6. Start Menu Overlay
    if (start_menu_open) {
        comp_draw_rect(16, 460, 320, 280, 0x1E293B);
        comp_draw_string(32, 480, "Veyro Start Menu", 0xFFFFFF);
        comp_draw_string(32, 520, "> Open Terminal", 0x38BDF8);
        comp_draw_string(32, 560, "> Open Notepad", 0xF43F5E);
        comp_draw_string(32, 600, "> Open File Explorer", 0xFBBF24);
        comp_draw_string(32, 640, "> Open System Monitor", 0x10B981);
        comp_draw_string(32, 690, "User: admin [Power / Settings]", 0x64748B);
    }

    // 7. Render Mouse Cursor
    struct mouse_state *m = get_mouse_state();
    comp_draw_cursor(m->x, m->y);

    compositor_swap_buffers();
}

void window_manager_handle_click(uint32_t x, uint32_t y) {
    if (x >= 16 && x <= 48 && y >= 760 && y <= 792) {
        start_menu_open = !start_menu_open;
    } else if (start_menu_open && x >= 32 && x <= 250) {
        if (y >= 520 && y <= 545) { app_term_open = true; start_menu_open = false; }
        else if (y >= 560 && y <= 585) { app_note_open = true; start_menu_open = false; }
        else if (y >= 600 && y <= 625) { app_expl_open = true; start_menu_open = false; }
        else if (y >= 640 && y <= 665) { app_sys_open = true; start_menu_open = false; }
    } else if (y >= 760 && y <= 792) {
        if (x >= 64 && x <= 96) app_term_open = !app_term_open;
        else if (x >= 104 && x <= 136) app_note_open = !app_note_open;
        else if (x >= 144 && x <= 176) app_expl_open = !app_expl_open;
        else if (x >= 184 && x <= 216) app_sys_open = !app_sys_open;
    } else if (x >= 32 && x <= 96) {
        if (y >= 48 && y <= 112) app_term_open = true;
        else if (y >= 144 && y <= 208) app_note_open = true;
        else if (y >= 240 && y <= 304) app_expl_open = true;
        else if (y >= 336 && y <= 400) app_sys_open = true;
    }
}

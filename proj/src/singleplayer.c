#include <lcom/lcf.h>

#include "singleplayer.h"

#include "proj.h"

#include "proj_macros.h"
#include "proj_func.h"
#include "ent.h"
#include "interrupts_func.h"

#include "libs.h"

#include "campaign.h"
#include "zombies.h"

int (singleplayer)(void) {

    int r;

    menu_t *main_menu = menu_ctor(font_get_default());
    menu_add_item(main_menu, "Campaign");
    menu_add_item(main_menu, "Zombies");
    menu_add_item(main_menu, "Back");

    //uint32_t refresh_count_value = sys_hz() / REFRESH_RATE;
    uint8_t last_lb = 0;
    struct packet pp;
    keys_t *keys = get_key_presses();

    /// loop stuff
    int click = 0;
    uint64_t int_vector = 0;
    int good = true;
    while (good) {
        /* Get a request message. */
        if((r = get_interrupts_vector(&int_vector))) return r;
        uint32_t n = 1;
        for (uint8_t i = 0; i < 32; i++, n <<= 1) {
            if (int_vector & n) {
                interrupt_handler(i);
                switch (i) {
                    case TIMER0_IRQ:

                    graph_clear_screen();
                    switch(menu_update_state(main_menu, click)){
                        case -1: break;
                        case  0: campaign(); break;
                        case  1: zombies(); break;
                        case  2: good = false; break;
                    }
                    menu_draw(main_menu);

                    click = 0;

                    sprite_set_pos(sp_crosshair, *get_mouse_X(), *get_mouse_Y());
                    sprite_draw(sp_crosshair);
                    graph_draw();

                    break;
                    case KBC_IRQ:
                    if (keyboard_get_scancode()[0] == ESC_BREAK_CODE) good = false;
                    case MOUSE_IRQ:
                    if (mouse_get_counter_mouse_ih() >= 3) {
                        mouse_parse_packet(mouse_get_packet_mouse_ih(), &pp);
                        update_mouse(&pp);
                        if (!click) click = last_lb ^ keys->lb_pressed && keys->lb_pressed;
                        last_lb = keys->lb_pressed;
                        mouse_set_counter_mouse_ih(0);
                    }
                    break;
                    case COM1_IRQ: nctp_ih(); break;
                }
            }
        }
    }

    return 0;
}

#include <lcom/lcf.h>

#include "campaign.h"

#include "proj.h"

#include "proj_macros.h"
#include "proj_func.h"
#include "ent.h"
#include "interrupts_func.h"

#include "libs.h"

#include <math.h>

int (campaign)(void){

    int r;

    ent_set_scale(DEFAULT_SCALE);
    text_timer_t *in_game_timer = text_timer_ctor(font_get_default());

    list_t *shooter_list = list_ctor();

    gunner_t *shooter1 = gunner_ctor(bsp_shooter, bsp_pistol, GUNNER_PLAYER, 1); if(shooter1 == NULL) printf("Failed to get shooter1\n");
    gunner_set_spawn(shooter1, 75, 75);
    gunner_set_pos(shooter1, 75, 75);

    gunner_t *shooter2 = gunner_ctor(bsp_shooter, bsp_nothing, GUNNER_PLAYER, 2);
    gunner_set_spawn(shooter2, 975, 75);
    gunner_set_pos(shooter2, 775, 75);

    list_insert(shooter_list, list_end(shooter_list), shooter1);
    list_insert(shooter_list, list_end(shooter_list), shooter2);

    list_t *bullet_list  = list_ctor();

    ent_set_origin(gunner_get_x(shooter1)-ent_get_XLength()/2.0,
    gunner_get_y(shooter1)-ent_get_YLength()/2.0);

    //uint32_t refresh_count_value = sys_hz() / REFRESH_RATE;
    uint8_t last_lb = 0;
    struct packet pp;
    keys_t *keys = get_key_presses();

    /// loop stuff
    uint64_t int_vector = 0;
    int good = true;
    while (good) {
        /* Get a request message. */
        if((r = get_interrupts_vector(&int_vector))) return r;
        uint32_t n = 1;
        for (uint8_t i = 0; i < 32; i++, n <<= 1) {
            if(!good) break;
            if (int_vector & n) {
                interrupt_handler(i);
                switch (i) {
                    case TIMER0_IRQ:

                    if (timer_get_no_interrupts() % 60 == 0) text_timer_update(in_game_timer);
                    update_movement(map1, shooter1, keys, shooter_list);

                    update_game_state(map1, shooter_list, bullet_list);

                    //update_scale();
                    double angle = get_mouse_angle(shooter1);
                    gunner_set_angle(shooter1, angle - M_PI_2);

                    ent_set_origin(gunner_get_x(shooter1)-ent_get_XLength()/2.0,
                    gunner_get_y(shooter1)-ent_get_YLength()/2.0);

                    graph_clear_screen();
                    map_draw   (map1);
                    bullet_draw_list(bullet_list);
                    gunner_draw_list(shooter_list);

                    text_draw(in_game_timer->text);

                    sprite_set_pos(sp_crosshair, *get_mouse_X(), *get_mouse_Y());
                    sprite_draw(sp_crosshair);
                    graph_draw();

                    break;
                    case KBC_IRQ:
                    if (keyboard_get_scancode()[0] == ESC_BREAK_CODE) {
                        good = false;
                    }
                    break;
                    case MOUSE_IRQ:
                    if (mouse_get_counter_mouse_ih() >= 3) {
                        mouse_parse_packet(mouse_get_packet_mouse_ih(), &pp);
                        update_mouse(&pp);
                        if (last_lb ^ keys->lb_pressed && keys->lb_pressed)
                        shoot_bullet(shooter1, bullet_list, bsp_bullet);
                        last_lb = keys->lb_pressed;
                        mouse_set_counter_mouse_ih(0);

                    }
                    break;
                    case COM1_IRQ: nctp_ih(); break;
                }
            }
        }
    }

    while(list_size(shooter_list) > 0){
        gunner_t *p = list_erase(shooter_list, list_begin(shooter_list));
        gunner_dtor(p);
    }
    if(list_dtor(shooter_list)) printf("COULD NOT DESTRUCT SHOOTER LIST\n");

    while(list_size(bullet_list) > 0){
        bullet_t *p = (bullet_t*)list_erase(bullet_list, list_begin(bullet_list));
        bullet_dtor(p);
    }
    if(list_dtor(bullet_list)) printf("COULD NOT DESTRUCT BULLET LIST\n");

    text_timer_dtor(in_game_timer); in_game_timer = NULL;

    return SUCCESS;
}

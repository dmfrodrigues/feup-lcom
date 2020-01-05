#include <lcom/lcf.h>

#include "zombies.h"

#include "proj.h"

#include "libs.h"

#include "proj_macros.h"
#include "proj_func.h"
#include "ent.h"
#include "interrupts_func.h"
#include <math.h>

#define ZOMBIES_NUM             5
#define ZOMBIE_HEALTH_FACTOR    1.1
int (zombies)(void){

    int r;

    ent_set_scale(DEFAULT_SCALE);
    text_timer_t *in_game_timer = text_timer_ctor(font_get_default());

    list_t *shooter_list = list_ctor();

    gunner_t *shooter1 = gunner_ctor(bsp_shooter, bsp_pistol, GUNNER_PLAYER, 1); if(shooter1 == NULL) printf("Failed to get shooter1\n");
    gunner_set_spawn(shooter1, 980, 790);
    gunner_set_pos(shooter1, 980, 790);

    list_insert(shooter_list, list_end(shooter_list), shooter1);

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
    int dead = false;

    int health = 50;

    map_make_dijkstra(map1, gunner_get_x(shooter1), gunner_get_y(shooter1));

    while (good && !dead) {
        /* Get a request message. */
        if((r = get_interrupts_vector(&int_vector))) return r;
        uint32_t n = 1;
        for (uint8_t i = 0; i < 32; i++, n <<= 1) {
            if(!good || dead) break;
            if (int_vector & n) {
                interrupt_handler(i);
                switch (i) {
                    case TIMER0_IRQ:
                    if (timer_get_no_interrupts() % 60 == 0) text_timer_update(in_game_timer);
                    if (timer_get_no_interrupts() %  6 == 0){
                        map_make_dijkstra(map1, gunner_get_x(shooter1), gunner_get_y(shooter1));
                    }

                    update_movement(map1, shooter1, keys, shooter_list);

                    update_game_state(map1, shooter_list, bullet_list);

                    if(list_find(shooter_list, shooter1) == list_end(shooter_list)){
                        good = false;
                        dead = true;
                        break;
                    }

                    double angle = get_mouse_angle(shooter1);
                    gunner_set_angle(shooter1, angle - M_PI_2);

                    ent_set_origin(gunner_get_x(shooter1)-ent_get_XLength()/2.0,
                                   gunner_get_y(shooter1)-ent_get_YLength()/2.0);

                    while(list_size(shooter_list) < ZOMBIES_NUM+1){
                        gunner_t *zombie = gunner_ctor(bsp_zombie, bsp_nothing, GUNNER_MELEE | GUNNER_FOLLOW, 3);
                        gunner_set_health(zombie, health);
                        gunner_set_curr_health(zombie, health);
                        health *= ZOMBIE_HEALTH_FACTOR;
                        get_random_spawn(map1, zombie, shooter_list);
                        list_push_back(shooter_list, zombie);
                    }

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

    if(dead){
        printf("YOU DIED\n");
    }

    text_timer_dtor(in_game_timer); in_game_timer = NULL;

    return SUCCESS;
}

#include <lcom/lcf.h>
#include <lcom/proj.h>
#include <lcom/liblm.h>
#include <math.h>

#include "proj_macros.h"
#include "proj_func.h"

#include "kbc.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "graph.h"
#include "menu.h"
#include "rtc.h"
#include "hltp.h"
#include "interrupts_func.h"
#include "makecode_map.h"

#include "graph.h"
#include "rectangle.h"
#include "font.h"
#include "ent.h"

#include "crosshair.h"
#include "shooter.h"
#include "pistol.h"
#include "nothing.h"
#include "bullet.h"
#include "map1.h"

#include "errors.h"

#include "list.h"

int main(int argc, char* argv[]) {

    lcf_set_language("EN-US");

    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

    lcf_log_output("/home/lcom/labs/proj/output.txt");

    if (lcf_start(argc, argv)) return 1;

    lcf_cleanup();

    return 0;
}

font_t               *consolas      = NULL;
basic_sprite_t       *bsp_crosshair = NULL;
basic_sprite_t       *bsp_shooter   = NULL;
basic_sprite_t       *bsp_pistol    = NULL;
basic_sprite_t       *bsp_nothing   = NULL;
basic_sprite_t       *bsp_bullet    = NULL;
map_t                *map1          = NULL;
sprite_t             *sp_crosshair  = NULL;

static int (game)(void);

int(proj_main_loop)(int argc, char *argv[]) {

    int r;

    consolas = font_ctor("/home/lcom/labs/proj/media/font/Consolas/xpm2");
    if(consolas == NULL){ printf("Failed to load consolas\n"); return 1; }

    /// subscribe interrupts
    if (subscribe_all()) { return 1; }

    #ifndef DIOGO
        /// initialize graphics
        if(graph_init(GRAPH_MODE)){
            printf("%s: failed to initalize graphics.\n", __func__);
            if (cleanup()) printf("%s: failed to cleanup.\n", __func__);
            return 1;
        }
    #endif

    /// Load stuff
    {
        #ifndef DIOGO
            graph_clear_screen();
            text_t *txt = text_ctor(consolas, "Loading...");
            text_draw(txt);
            text_dtor(txt);
            graph_draw();
        #endif

        bsp_crosshair = get_crosshair(); if(bsp_crosshair == NULL) printf("Failed to get crosshair\n");
        bsp_shooter   = get_shooter  (); if(bsp_shooter   == NULL) printf("Failed to get shooter\n");
        bsp_pistol    = get_pistol   (); if(bsp_pistol    == NULL) printf("Failed to get pistol\n");
        bsp_nothing   = get_nothing  (); if(bsp_nothing   == NULL) printf("Failed to get nothing\n");
        bsp_bullet    = get_bullet   (); if(bsp_bullet    == NULL) printf("Failed to get bullet\n");
        map1          = get_map1     (); if(map1          == NULL) printf("Failed to get map1\n");

        sp_crosshair = sprite_ctor(bsp_crosshair); if(sp_crosshair == NULL) printf("Failed to get crosshair sprite\n");
    }

    ent_set_scale(DEFAULT_SCALE);

    text_timer_t *in_game_timer = timer_ctor(consolas);

    #ifndef DIOGO
        menu_t *main_menu = menu_ctor(consolas);
    #endif

    list_t *shooter_list = list_ctor();

    gunner_t *shooter1 = gunner_ctor(bsp_shooter, bsp_pistol); if(shooter1 == NULL) printf("Failed to get shooter1\n");
    gunner_set_spawn(shooter1, 75, 75);
    gunner_set_pos(shooter1, 75, 75);

    gunner_t *shooter2 = gunner_ctor(bsp_shooter, bsp_nothing);
    gunner_set_spawn(shooter2, 975, 75);
    gunner_set_pos(shooter2, 775, 75);

    list_insert(shooter_list, list_end(shooter_list), shooter1);
    list_insert(shooter_list, list_end(shooter_list), shooter2);

    list_t *bullet_list  = list_ctor();

    ent_set_origin(gunner_get_x(shooter1)-ent_get_XLength()/2.0,
                   gunner_get_y(shooter1)-ent_get_YLength()/2.0);

    #ifndef DIOGO
        //uint32_t refresh_count_value = sys_hz() / REFRESH_RATE;
        uint8_t last_lb = 0;
        struct packet pp;
        keys_t *keys = get_key_presses();
    #endif
    /// loop stuff
    int ipc_status;
    message msg;
    int game_state = MENU;

    #ifdef DIOGO
        char buffer[1024]; // buffer
        int buffer_pos = 0;
    #endif

    #ifndef DIOGO
        int click = 0;
    #endif
    #ifdef DIOGO
        char *s = NULL;
    #endif

    while (game_state != EXIT) {
        /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    for (uint32_t i = 0, n = 1; i < 32; i++, n <<= 1) {
                        if (msg.m_notify.interrupts & n) {
                            interrupt_handler(i);
                            switch (i) {
                            #ifndef DIOGO
                            case TIMER0_IRQ:
                                switch (game_state) {
                                case MENU:
                                    graph_clear_screen();
                                    switch(menu_update_state(main_menu, click)){
                                        case -1: game_state = MENU; break;
                                        case  0: game_state = GAME; game(); break;
                                        case  1: game_state = TEST; break;
                                        case  2: game_state = EXIT; break;
                                    }
                                    menu_draw(main_menu);

                                    click = 0;

                                    sprite_set_pos(sp_crosshair, *get_mouse_X(), *get_mouse_Y());
                                    sprite_draw(sp_crosshair);
                                    graph_draw();
                                    break;
                                case GAME:
                                    if (no_interrupts % 60 == 0) timer_update(in_game_timer);
                                    update_movement(map1, shooter1, keys, shooter_list);

                                    update_game_state(map1, shooter_list, bullet_list);

                                    //update_scale();
                                    double angle = get_mouse_angle(shooter1);
                                    gunner_set_angle(shooter1, angle - M_PI_2);

                                    ent_set_origin(gunner_get_x(shooter1)-ent_get_XLength()/2.0,
                                                   gunner_get_y(shooter1)-ent_get_YLength()/2.0);

                                    graph_clear_screen();
                                    map_draw   (map1);
                                    gunner_draw_list(shooter_list);
                                    bullet_draw_list(bullet_list);

                                    text_draw(in_game_timer->text);

                                    sprite_set_pos(sp_crosshair, *get_mouse_X(), *get_mouse_Y());
                                    sprite_draw(sp_crosshair);
                                    graph_draw();
                                    break;
                                }
                                break;
                            #endif
                            case KBC_IRQ:
                                switch (game_state) {
                                case MENU:
                                    if ((scancode[0]) == ESC_BREAK_CODE) game_state = EXIT;
                                    #ifdef DIOGO
                                    else if ((scancode[0]) == ENTER_MAKE_CODE) {
                                        buffer[buffer_pos] = '\0';
                                        printf("\nSending string -%s-", buffer);
                                        printf(" (output: %d)\n",
                                            hltp_send_string(buffer));
                                        buffer_pos = 0;
                                    }
                                    else {
                                        char c = map_makecode(scancode[0]);
                                        if (c == ERROR_CODE) break;
                                        buffer[buffer_pos++] = c;
                                        printf("%c", c);
                                    }
                                    #endif
                                    break;
                                case GAME:
                                    if ((scancode[0]) == ESC_BREAK_CODE) {
                                        game_state = MENU;
                                        // reset game
                                        while(list_size(bullet_list) > 0){
                                            bullet_t *p = (bullet_t*)list_erase(bullet_list, list_begin(bullet_list));
                                            bullet_dtor(p);
                                        }
                                        list_node_t *it = list_begin(shooter_list);
                                        while (it != list_end(shooter_list)) {
                                            gunner_t *p = *(gunner_t**)list_node_val(it);
                                            get_random_spawn(map1, p);
                                            gunner_set_curr_health(p, gunner_get_health(p));
                                            it = list_node_next(it);
                                        }
                                        timer_reset(in_game_timer);
                                    }
                                    break;
                                }
                                break;
                            #ifndef DIOGO
                            case MOUSE_IRQ:
                                if (counter_mouse_ih >= 3) {
                                    mouse_parse_packet(packet_mouse_ih, &pp);
                                    update_mouse(&pp);
                                    switch (game_state) {
                                    case MENU:
                                        if (!click) click = last_lb ^ keys->lb_pressed && keys->lb_pressed;
                                        break;
                                    case GAME:
                                        if (last_lb ^ keys->lb_pressed && keys->lb_pressed)
                                            shoot_bullet(shooter1, bullet_list, bsp_bullet);
                                        break;
                                    }
                                    last_lb = keys->lb_pressed;
                                    counter_mouse_ih = 0;

                                }
                                break;
                            #endif
                            #ifdef DIOGO
                            case COM1_IRQ:
                                nctp_ih();
                                break;
                            #endif
                            }
                        }
                    }

                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else { /* received standart message, not a notification */
            /* no standart message expected: do nothing */
        }
    }

    #ifdef DIOGO
        free(s);
    #endif

    while(list_size(shooter_list) > 0){
        gunner_t *p = list_erase(shooter_list, list_begin(shooter_list));
        gunner_dtor(p);
    }

    while(list_size(bullet_list) > 0){
        bullet_t *p = (bullet_t*)list_erase(bullet_list, list_begin(bullet_list));
        bullet_dtor(p);
    }
    if(list_dtor(shooter_list)) printf("COULD NOT DESTRUCT SHOOTER LIST\n");
    if(list_dtor(bullet_list)) printf("COULD NOT DESTRUCT BULLET LIST\n");

    timer_dtor(in_game_timer); in_game_timer = NULL;

    basic_sprite_dtor      (bsp_crosshair); bsp_crosshair = NULL;
    basic_sprite_dtor      (bsp_shooter  ); bsp_shooter   = NULL;
    sprite_dtor            (sp_crosshair ); sp_crosshair  = NULL;
    basic_sprite_dtor      (bsp_pistol   ); bsp_pistol    = NULL;
    basic_sprite_dtor      (bsp_nothing  ); bsp_nothing   = NULL;
    map_dtor               (map1         ); map1          = NULL;
    font_dtor              (consolas     ); consolas      = NULL;

    // Unsubscribe interrupts
    if (unsubscribe_all()) {
        if (cleanup())
            printf("%s: failed to cleanup.\n", __func__);
        return 1;
    }


    if (cleanup()) {
        printf("%s: failed to cleanup.\n", __func__);
        return 1;
    }

    return 0;
}

static int (game)(void){
    return SUCCESS;
}

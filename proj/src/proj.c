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
#include "rtc.h"
#include "interrupts_func.h"

#include "graph.h"
#include "sprite.h"
#include "rectangle.h"
#include "font.h"
#include "ent.h"

#include "crosshair.h"
#include "shooter.h"
#include "pistol.h"
#include "nothing.h"
#include "bullet.h"
#include "map1.h"

#include "list.h"

#ifdef DIOGO
    #include "uart_macros.h"
    #include "test7.h"
#endif

int main(int argc, char* argv[]) {

    lcf_set_language("EN-US");

    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

    lcf_log_output("/home/lcom/labs/proj/output.txt");

    if (lcf_start(argc, argv)) return 1;

    lcf_cleanup();

    return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {

    int r;

    #ifdef DIOGO
        /*
        uint8_t conf;
        if((r = util_sys_inb(0x3F8+3, &conf))) return 1; //printf("0x%02X\n", conf);
        conf = 0x19; //00011001
        //printf("0x%02X\n", conf);
        if((r = sys_outb(0x3F8+3, conf))) return 1;
        if((r = util_sys_inb(0x3F8+3, &conf))) return 1; //printf("0x%02X\n", conf);
        */
    #endif

    font_t *consolas = font_ctor("/home/lcom/labs/proj/media/font/Consolas/xpm2");
    if(consolas == NULL){ printf("Failed to load consolas\n"); return 1; }

    /// subscribe interrupts
    if (subscribe_all()) { return 1; }

    /// initialize graphics
    if(graph_init(GRAPH_MODE)){
        printf("%s: failed to initalize graphics.\n", __func__);
        if (cleanup()) printf("%s: failed to cleanup.\n", __func__);
        return 1;
    }

    /// Load stuff
    basic_sprite_t       *bsp_crosshair = NULL;
    basic_sprite_t       *bsp_shooter   = NULL;
    basic_sprite_t       *bsp_pistol    = NULL;
    basic_sprite_t       *bsp_nothing   = NULL;
    basic_sprite_t       *bsp_bullet    = NULL;
    map_t                *map1          = NULL;
    sprite_t             *sp_crosshair  = NULL;
    {
        graph_clear_screen();
        text_t *txt = text_ctor(consolas, "Loading...");
        text_draw(txt);
        text_dtor(txt);
        graph_draw();

        bsp_crosshair = get_crosshair(); if(bsp_crosshair == NULL) printf("Failed to get crosshair\n");
        bsp_shooter   = get_shooter  (); if(bsp_shooter   == NULL) printf("Failed to get shooter\n");
        bsp_pistol    = get_pistol   (); if(bsp_pistol    == NULL) printf("Failed to get pistol\n");
        bsp_nothing   = get_nothing  (); if(bsp_nothing   == NULL) printf("Failed to get nothing\n");
        bsp_bullet    = get_bullet   (); if(bsp_bullet    == NULL) printf("Failed to get bullet\n");
        map1          = get_map1     (); if(map1          == NULL) printf("Failed to get map1\n");

        sp_crosshair = sprite_ctor(bsp_crosshair); if(sp_crosshair == NULL) printf("Failed to get crosshair sprite\n");
    }

    #ifdef DIOGO
        /*
        graph_clear_screen();

        rectangle_t *rect = rectangle_ctor(0,0,400,100);
        rectangle_set_pos(rect,
                          graph_get_XRes()/2    - rectangle_get_w(rect)/2,
                          graph_get_YRes()*0.25 - rectangle_get_h(rect)/2);
        rectangle_set_fill_color(rect, BLACK);
        rectangle_set_outline_width(rect, 2);
        rectangle_set_outline_color(rect, WHITE);
        rectangle_draw(rect);

        text_t *txt  = text_ctor(consolas, "Hello world!");
        text_set_color(txt, 0x888888);

        text_set_pos(txt, rectangle_get_x(rect)+rectangle_get_w(rect)/2,
                          rectangle_get_y(rect)+rectangle_get_h(rect)/2);
        text_set_valign(txt, text_valign_center);
        text_set_halign(txt, text_halign_center);
        text_draw(txt);
        text_dtor(txt);

        graph_draw();
        rectangle_dtor(rect);

        list_t *l = list_ctor();
        int *p = NULL;
        for(int i = 10; i < 20; ++i){
            p = malloc(sizeof(int));
            *p = i;
            printf("INSERTING %d\n", i);
            list_insert(l, list_end(l), p);
            printf("INSERTED, SIZE=%d\n", list_size(l));
        }
        list_node_t *it = list_begin(l);
        while(it != list_end(l)){
            printf("%d\n", **(int**)list_node_val(it));
            it = list_node_next(it);
        }
        while(list_size(l) > 0){
            printf("ERASING\n");
            void *p = list_erase(l, list_begin(l));
            printf("ERASED %d, SIZE=%d\n", *(int*)p, list_size(l));
            free(p);
        }
        printf("DONE\n");
        if(list_dtor(l)) printf("COULD NOT DESTRUCT LIST\n");


        tickdelay(micros_to_ticks(1000000));*/

        // RTC
        /*
        uint8_t date[4], time[3];

        if (rtc_read_time(time)) return 1;

        if (rtc_read_date(date)) return 1;

        printf("Hour: %02d:%02d:%02d\n", time[2], time[1], time[0]);

        printf("Date: %d, %02d/%02d/%02d\n", date[0], date[1], date[2], date[3]);
        */
        //UART
        /*
        printf("got %d\n", ser_test_conf(COM1_ADDR));
        printf("\n");
        printf("got %d\n", ser_test_set(COM1_ADDR, 6, 1, 0, 9800));
        tickdelay(micros_to_ticks(1000000));
        printf("got %d\n", ser_test_conf(COM1_ADDR));
        */


    #endif

    #ifdef TELMO
        ent_set_scale(DEFAULT_SCALE);

        list_t *shooter_list = list_ctor();

        gunner_t *shooter1 = gunner_ctor(bsp_shooter, bsp_pistol); if(shooter1 == NULL) printf("Failed to get shooter1\n");
        gunner_set_spawn(shooter1, 75, 75);
        gunner_set_pos(shooter1, 75, 75);

        gunner_t *shooter2 = gunner_ctor(bsp_shooter, bsp_nothing);
        gunner_set_spawn(shooter2, 975, 75);
        gunner_set_pos(shooter2, 775, 75);

        list_insert(shooter_list, list_end(shooter_list), shooter1);
        list_insert(shooter_list, list_end(shooter_list), shooter2);

        //bullet_t *bullet = bullet_ctor(get_bullet(), 400.0, 400.0, 2.0, -1.0);

        list_t *bullet_list  = list_ctor();

        ent_set_origin(gunner_get_x(shooter1)-ent_get_XLength()/2.0,
                       gunner_get_y(shooter1)-ent_get_YLength()/2.0);

        uint32_t refresh_count_value = sys_hz() / REFRESH_RATE;

        uint8_t last_lb = 0;
    #endif

    /// loop stuff
    int ipc_status;
    message msg;
    int good = 1;

    #ifdef DIOGO
        good = 0;
    #endif

    while (good) {
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
                            if ((scancode[0]) == ESC_BREAK_CODE) good = 0;
                            #ifdef TELMO
                            if (i == 0) {
                                if (no_interrupts % refresh_count_value == 0) {
                                    //update_movement(map1, shooter_list);
                                    //bullet_update_movement(bullet);

                                    if (no_interrupts % 180 == 0) gunner_set_pos(shooter2, 775, 75);

                                    update_game_state(map1, shooter_list, bullet_list);

                                    if(map_collides_gunner(map1, shooter1)){
                                        printf("COLLIDING\n");
                                    }

                                    update_scale();

                                    ent_set_origin(gunner_get_x(shooter1)-ent_get_XLength()/2.0,
                                                   gunner_get_y(shooter1)-ent_get_YLength()/2.0);

                                    sprite_set_pos(sp_crosshair, get_mouse_X(), get_mouse_Y());
                                    double angle = get_mouse_angle(shooter1);
                                    gunner_set_angle(shooter1, angle - M_PI_2);
                                    graph_clear_screen();

                                    clock_t t = clock();

                                    map_draw   (map1);
                                    gunner_draw(shooter2);
                                    gunner_draw(shooter1);
                                    bullet_draw_list(bullet_list);

                                    t = clock()-t; //printf("%d\n", (t*1000)/CLOCKS_PER_SEC);

                                    sprite_draw(sp_crosshair);
                                    graph_draw();
                                }
                            }
                            if (i == 12) {
                                if (counter_mouse_ih >= 3) {
                                    struct packet pp = mouse_parse_packet(packet_mouse_ih);
                                    update_mouse(&pp);
                                    //printf("X: %d Y: %d\n", get_mouse_X(), get_mouse_Y());
                                    counter_mouse_ih = 0;

                                    if (last_lb ^ get_key_presses()->lb_pressed && get_key_presses()->lb_pressed) {
                                        shoot_bullet(shooter1, bullet_list, bsp_bullet);
                                    }

                                    last_lb = get_key_presses()->lb_pressed;
                                }
                            }
                            #endif
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

    #ifdef TELMO
        while(list_size(shooter_list) > 0){
            gunner_t *p = list_erase(shooter_list, list_begin(shooter_list));
            gunner_dtor(p);
        }

        while(list_size(bullet_list) > 0){
            bullet_t *p = (bullet_t*)list_erase(bullet_list, list_begin(bullet_list));
            bullet_dtor(p);
        }
        if(list_dtor(bullet_list)) printf("COULD NOT DESTRUCT BULLET LIST\n");
    #endif

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

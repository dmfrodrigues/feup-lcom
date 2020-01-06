#include <lcom/lcf.h>
#include <lcom/proj.h>
#include <lcom/liblm.h>

#include "proj.h"

#include <math.h>

#include "proj_macros.h"
#include "proj_func.h"

#include "interrupts_func.h"
#include "makecode_map.h"

#include "crosshair.h"
#include "shooter.h"
#include "zombie.h"
#include "pistol.h"
#include "nothing.h"
#include "bullet.h"
#include "map1.h"

#include "hltp.h"


int main(int argc, char* argv[]) {

    lcf_set_language("EN-US");

    lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

    lcf_log_output("/home/lcom/labs/proj/src/output.txt");

    if (lcf_start(argc, argv)) return 1;

    lcf_cleanup();

    return 0;
}

#include "singleplayer.h"
static int (multiplayer)(void);
#include "chat.h"
int(proj_main_loop)(int argc, char *argv[]) {
    (void)argc; (void)argv;

    int r;

    if(font_init()){ printf("Failed to initialize fonts\n"); return 1; }

    /// subscribe interrupts
    if (subscribe_all()) { return 1; }

    /// initialize graphics
    if(graph_init(GRAPH_MODE)){
        printf("%s: failed to initalize graphics.\n", __func__);
        if (cleanup()) printf("%s: failed to cleanup.\n", __func__);
        return 1;
    }

    /// Load stuff
    {
        graph_clear_screen();
        text_t *txt = text_ctor(font_get_default(), "Loading...");
        text_set_pos(txt, graph_get_XRes()/2, graph_get_YRes()/2);
        text_set_valign(txt, text_valign_center);
        text_set_halign(txt, text_halign_center);
        text_set_color(txt, TEXT_COLOR);
        text_draw(txt);
        text_dtor(txt);
        graph_draw();

        bsp_crosshair = get_crosshair(); if(bsp_crosshair == NULL) printf("Failed to get crosshair\n");
        bsp_shooter   = get_shooter  (); if(bsp_shooter   == NULL) printf("Failed to get shooter\n");
        bsp_zombie    = get_zombie   (); if(bsp_zombie    == NULL) printf("Failed to get zombie\n");
        bsp_pistol    = get_pistol   (); if(bsp_pistol    == NULL) printf("Failed to get pistol\n");
        bsp_nothing   = get_nothing  (); if(bsp_nothing   == NULL) printf("Failed to get nothing\n");
        bsp_bullet    = get_bullet   (); if(bsp_bullet    == NULL) printf("Failed to get bullet\n");
        map1          = get_map1     (); if(map1          == NULL) printf("Failed to get map1\n");

        sp_crosshair = sprite_ctor(bsp_crosshair); if(sp_crosshair == NULL) printf("Failed to get crosshair sprite\n");
    }

    text_t *title     = text_ctor(font_get_default(), "LabWars");
    text_set_color(title, TEXT_COLOR);
    text_set_size(title, 70);
    text_set_pos(title, graph_get_XRes()/2, graph_get_YRes()*0.17);
    text_set_valign(title, text_valign_center);
    text_set_halign(title, text_halign_center);

    menu_t *main_menu = menu_ctor(font_get_default());
    menu_add_item(main_menu, "Single player");
    menu_add_item(main_menu, "Multiplayer");
    menu_add_item(main_menu, "Chat");
    menu_add_item(main_menu, "Exit");

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
                        case  0: singleplayer(); break; //campaign(); break;
                        case  1: multiplayer() ; break;
                        case  2: chat(); break;
                        case  3: good = false; break;
                    }
                    menu_draw(main_menu);
                    text_draw(title);

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

    text_dtor(title);
    menu_dtor(main_menu);

    basic_sprite_dtor      (bsp_crosshair); bsp_crosshair = NULL;
    basic_sprite_dtor      (bsp_shooter  ); bsp_shooter   = NULL;
    basic_sprite_dtor      (bsp_zombie   ); bsp_zombie    = NULL;
    sprite_dtor            (sp_crosshair ); sp_crosshair  = NULL;
    basic_sprite_dtor      (bsp_pistol   ); bsp_pistol    = NULL;
    basic_sprite_dtor      (bsp_nothing  ); bsp_nothing   = NULL;
    map_dtor               (map1         ); map1          = NULL;
    font_free();

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

static host_info_t     *host_info   = NULL;
static remote_info_t   *remote_info = NULL;
static bullet_info_t   *bullet_info = NULL;

static void multiplayer_process(const uint8_t *p, const size_t sz) {
    void *dest = NULL;
    hltp_type tp = hltp_interpret(p, sz, &dest);
    if (dest == NULL) return;
    switch(tp){
        case hltp_type_host:
            host_info = (host_info_t*)dest;
            break;
        case hltp_type_remote:
            remote_info = (remote_info_t*)dest;
            break;
        case hltp_type_bullet:
            bullet_info = (bullet_info_t*)dest;
            break;
        case hltp_type_invalid: break;
        case hltp_type_string : break;
    }
}
static int (multiplayer_host)(void);
static int (multiplayer_remote)(void);
static int (multiplayer)(void) {
    int r;

    menu_t *main_menu = menu_ctor(font_get_default());
    menu_add_item(main_menu, "Create");
    menu_add_item(main_menu, "Connect");
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
                        case  0: multiplayer_host(); break;
                        case  1: multiplayer_remote(); break;
                        case  2: good = false; break;
                    }
                    menu_draw(main_menu);

                    click = 0;

                    sprite_set_pos(sp_crosshair, *get_mouse_X(), *get_mouse_Y());
                    sprite_draw(sp_crosshair);
                    graph_draw();

                    break;
                    case KBC_IRQ:
                    if(!keyboard_get_done()) break;
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

static int (multiplayer_host)(void) {
    int r;

    nctp_dump();
    nctp_set_processor(multiplayer_process);

    ent_set_scale(DEFAULT_SCALE);
    text_timer_t *in_game_timer = text_timer_ctor(font_get_default());

    list_t *shooter_list = list_ctor();

    gunner_t *shooter1 = gunner_ctor(bsp_shooter, bsp_pistol, GUNNER_PLAYER, 1); if(shooter1 == NULL) printf("Failed to get shooter1\n");
    gunner_set_spawn(shooter1, 75, 75);

    gunner_t *shooter2 = gunner_ctor(bsp_shooter, bsp_pistol, GUNNER_PLAYER, 2); if(shooter2 == NULL) printf("Failed to get shooter2\n");
    gunner_set_spawn(shooter2, 975, 75);

    list_insert(shooter_list, list_end(shooter_list), shooter1);
    list_insert(shooter_list, list_end(shooter_list), shooter2);

    do {
        get_random_spawn(map1, shooter1, shooter_list);
        get_random_spawn(map1, shooter2, shooter_list);
    } while (gunner_distance(shooter1, shooter2) < 500);

    host_info = host_info_ctor(shooter1, shooter2);
    remote_info = remote_info_ctor();
    bullet_info = bullet_info_ctor();

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
    int state = 0; // -1 for remote win, 0 for draw, 1 for host win
    list_node_t *p1, *p2; // player states
    int state_1, state_2;
    while (good) {
        if ((r = get_interrupts_vector(&int_vector))) return r;
        uint32_t n = 1;
        for (uint8_t i = 0; i < 32; i++, n <<= 1) {
            if (int_vector & n) {
                interrupt_handler(i);
                switch (i) {
                    case TIMER0_IRQ:
                    if (timer_get_no_interrupts() % 60 == 0) text_timer_update(in_game_timer);
                    update_movement(map1, shooter1, keys, shooter_list);
                    update_movement(map1, shooter2, &(remote_info->remote_keys_pressed), shooter_list);

                    update_game_state(map1, shooter_list, bullet_list);

                    p1 = list_find(shooter_list, shooter1);
                    p2 = list_find(shooter_list, shooter2);

                    if ((state_1 = (p1 == list_end(shooter_list))) || (state_2 = (p2 == list_end(shooter_list)))) {
                        state = state_1 - state_2;
                        good = false;
                        break;
                    }

                    double angle = get_mouse_angle(shooter1);
                    gunner_set_angle(shooter1, angle - M_PI_2);

                    ent_set_origin(gunner_get_x(shooter1)-ent_get_XLength()/2.0,
                                   gunner_get_y(shooter1)-ent_get_YLength()/2.0);

                    gunner_set_angle(shooter2, remote_info->remote_angle);
                    //build_host_structure(host_info, shooter1, shooter2);

                    //hltp_send_host_info(host_info);

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
                    if(!keyboard_get_done()) break;
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

                    case COM1_IRQ:
                        nctp_ih();
                        if (bullet_info->new_bullet) {
                            shoot_bullet(shooter2, bullet_list, bsp_bullet);
                            bullet_info->new_bullet = false;
                        }
                        break;
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

    host_info_dtor(host_info);
    remote_info_dtor(remote_info);
    bullet_info_dtor(bullet_info);

    nctp_set_processor(NULL);

    text_timer_dtor(in_game_timer); in_game_timer = NULL;

    return 0;
}
static int (multiplayer_remote)(void) {
    int r;

    nctp_dump();
    nctp_set_processor(multiplayer_process);

    ent_set_scale(DEFAULT_SCALE);
    text_timer_t *in_game_timer = text_timer_ctor(font_get_default());

    list_t *shooter_list = list_ctor();

    gunner_t *shooter1 = gunner_ctor(bsp_shooter, bsp_pistol, GUNNER_PLAYER, 2); if(shooter1 == NULL) printf("Failed to get shooter1\n");
    gunner_set_spawn(shooter1, 75, 75);

    gunner_t *shooter2 = gunner_ctor(bsp_shooter, bsp_pistol, GUNNER_PLAYER, 1); if(shooter2 == NULL) printf("Failed to get shooter2\n");
    gunner_set_spawn(shooter2, 975, 75);

    list_insert(shooter_list, list_end(shooter_list), shooter1);
    list_insert(shooter_list, list_end(shooter_list), shooter2);

    host_info = host_info_ctor(shooter2, shooter1);
    remote_info = remote_info_ctor();
    bullet_info = bullet_info_ctor();

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
        if ((r = get_interrupts_vector(&int_vector))) return r;
        uint32_t n = 1;
        for (uint8_t i = 0; i < 32; i++, n <<= 1) {
            if (int_vector & n) {
                interrupt_handler(i);
                switch (i) {
                    case TIMER0_IRQ:
                    if (timer_get_no_interrupts() % 60 == 0) text_timer_update(in_game_timer);

                    double angle = get_mouse_angle(shooter1) - M_PI_2;

                    build_remote_structure(remote_info, get_key_presses(), angle);
                    hltp_send_remote_info(remote_info);


                    gunner_set_pos(shooter1, (double)host_info->remote_x, (double)host_info->remote_y);
                    gunner_set_angle(shooter1, (double)host_info->remote_angle);
                    gunner_set_health(shooter1, (double)host_info->remote_health);
                    gunner_set_curr_health(shooter1, (double)host_info->remote_current_health);

                    gunner_set_pos(shooter2, (double)host_info->host_x, (double)host_info->host_y);
                    gunner_set_angle(shooter2, (double)host_info->host_angle);
                    gunner_set_health(shooter2, (double)host_info->host_health);
                    gunner_set_curr_health(shooter2, (double)host_info->host_current_health);

                    ent_set_origin(gunner_get_x(shooter1)-ent_get_XLength()/2.0,
                                   gunner_get_y(shooter1)-ent_get_YLength()/2.0);

                    /*
                    for (size_t j = 0; j < host_info->no_bullets; j++) {
                        if (host_info->bullets_shooter[j]) { // remote
                            bullet_t *bullet = bullet_ctor(shooter1, bsp_bullet, (double)host_info->bullets_x[j], (double)host_info->bullets_y[j], (double)host_info->bullets_vx[j], (double)host_info->bullets_vy[j]);
                            list_insert(bullet_list, list_end(bullet_list), bullet);
                        } else { // host
                            bullet_t *bullet = bullet_ctor(shooter2, bsp_bullet, (double)host_info->bullets_x[j], (double)host_info->bullets_y[j], (double)host_info->bullets_vx[j], (double)host_info->bullets_vy[j]);
                            list_insert(bullet_list, list_end(bullet_list), bullet);
                        }
                    }*/

                    graph_clear_screen();
                    map_draw   (map1);
                    bullet_draw_list(bullet_list);
                    gunner_draw_list(shooter_list);

                    text_draw(in_game_timer->text);

                    sprite_set_pos(sp_crosshair, *get_mouse_X(), *get_mouse_Y());
                    sprite_draw(sp_crosshair);
                    graph_draw();
                    /*
                    while(list_size(bullet_list) > 0){
                        bullet_t *p = (bullet_t*)list_erase(bullet_list, list_begin(bullet_list));
                        bullet_dtor(p);
                    }
                    */

                    break;
                    case KBC_IRQ:
                    if(!keyboard_get_done()) break;
                    if (keyboard_get_scancode()[0] == ESC_BREAK_CODE) {
                        good = false;
                    }
                    break;
                    case MOUSE_IRQ:
                    if (mouse_get_counter_mouse_ih() >= 3) {
                        mouse_parse_packet(mouse_get_packet_mouse_ih(), &pp);
                        update_mouse(&pp);
                        if (last_lb ^ keys->lb_pressed && keys->lb_pressed) {
                            bullet_info->new_bullet = true;
                            hltp_send_bullet_info(bullet_info);
                        }
                        last_lb = keys->lb_pressed;
                        mouse_set_counter_mouse_ih(0);
                    }
                    break;

                    case COM1_IRQ:
                        nctp_ih();
                        break;
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

    host_info_dtor(host_info);
    remote_info_dtor(remote_info);
    bullet_info_dtor(bullet_info);

    nctp_set_processor(NULL);

    text_timer_dtor(in_game_timer); in_game_timer = NULL;

    return 0;
}

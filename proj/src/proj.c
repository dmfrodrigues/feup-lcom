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
#include "zombie.h"
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
basic_sprite_t       *bsp_zombie    = NULL;
basic_sprite_t       *bsp_pistol    = NULL;
basic_sprite_t       *bsp_nothing   = NULL;
basic_sprite_t       *bsp_bullet    = NULL;
map_t                *map1          = NULL;
sprite_t             *sp_crosshair  = NULL;

static int (singleplayer)(void);
static int (chat)(void);
int(proj_main_loop)(int argc, char *argv[]) {

    int r;

    consolas = font_ctor("/home/lcom/labs/proj/media/font/Consolas/xpm2");
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
    {
        graph_clear_screen();
        text_t *txt = text_ctor(consolas, "Loading...");
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

    menu_t *main_menu = menu_ctor(consolas);
    menu_add_item(main_menu, "Single player");
    menu_add_item(main_menu, "Multiplayer");
    menu_add_item(main_menu, "Chat");
    menu_add_item(main_menu, "Exit");

    //uint32_t refresh_count_value = sys_hz() / REFRESH_RATE;
    uint8_t last_lb = 0;
    struct packet pp;
    keys_t *keys = get_key_presses();

    /// loop stuff
    int ipc_status;
    message msg;

    int click = 0;

    int good = true;

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
                            switch (i) {
                            case TIMER0_IRQ:

                                graph_clear_screen();
                                switch(menu_update_state(main_menu, click)){
                                    case -1: break;
                                    case  0: singleplayer(); break; //campaign(); break;
                                    case  1: break;
                                    case  2: chat(); break;
                                    case  3: good = false; break;
                                }
                                menu_draw(main_menu);

                                click = 0;

                                sprite_set_pos(sp_crosshair, *get_mouse_X(), *get_mouse_Y());
                                sprite_draw(sp_crosshair);
                                graph_draw();

                                break;
                            case KBC_IRQ:
                                if ((scancode[0]) == ESC_BREAK_CODE) good = false;
                            case MOUSE_IRQ:
                                if (counter_mouse_ih >= 3) {
                                    mouse_parse_packet(packet_mouse_ih, &pp);
                                    update_mouse(&pp);
                                    if (!click) click = last_lb ^ keys->lb_pressed && keys->lb_pressed;
                                    last_lb = keys->lb_pressed;
                                    counter_mouse_ih = 0;
                                }
                                break;
                            case COM1_IRQ: nctp_ih(); break;
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

    basic_sprite_dtor      (bsp_crosshair); bsp_crosshair = NULL;
    basic_sprite_dtor      (bsp_shooter  ); bsp_shooter   = NULL;
    /*basic_sprite_dtor      (bsp_zombie   );*/ bsp_zombie    = NULL;
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

static int (campaign)(void);
static int (zombies)(void);
static int (singleplayer)(void) {

    int r;

    menu_t *main_menu = menu_ctor(consolas);
    menu_add_item(main_menu, "Campaign");
    menu_add_item(main_menu, "Zombies");
    menu_add_item(main_menu, "Back");

    //uint32_t refresh_count_value = sys_hz() / REFRESH_RATE;
    uint8_t last_lb = 0;
    struct packet pp;
    keys_t *keys = get_key_presses();

    /// loop stuff
    int click = 0;
    uint32_t int_vector = 0;
    int good = true;
    while (good) {
        /* Get a request message. */
        if((r = get_interrupts_vector(&int_vector))) return r;
        for (uint32_t i = 0, n = 1; i < 32; i++, n <<= 1) {
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
                    if ((scancode[0]) == ESC_BREAK_CODE) good = false;
                case MOUSE_IRQ:
                    if (counter_mouse_ih >= 3) {
                        mouse_parse_packet(packet_mouse_ih, &pp);
                        update_mouse(&pp);
                        if (!click) click = last_lb ^ keys->lb_pressed && keys->lb_pressed;
                        last_lb = keys->lb_pressed;
                        counter_mouse_ih = 0;
                    }
                    break;
                case COM1_IRQ: nctp_ih(); break;
                }
            }
        }
    }

    return 0;
}

static int (campaign)(void){

    int r;

    ent_set_scale(DEFAULT_SCALE);
    text_timer_t *in_game_timer = timer_ctor(consolas);

    list_t *shooter_list = list_ctor();

    gunner_t *shooter1 = gunner_ctor(bsp_shooter, bsp_pistol, gunner_player, 1); if(shooter1 == NULL) printf("Failed to get shooter1\n");
    gunner_set_spawn(shooter1, 75, 75);
    gunner_set_pos(shooter1, 75, 75);

    gunner_t *shooter2 = gunner_ctor(bsp_shooter, bsp_nothing, gunner_player, 2);
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
    uint32_t int_vector = 0;
    int good = true;
    while (good) {
        /* Get a request message. */
        if((r = get_interrupts_vector(&int_vector))) return r;
        for (uint32_t i = 0, n = 1; i < 32; i++, n <<= 1) {
           if (int_vector & n) {
               interrupt_handler(i);
               switch (i) {
               case TIMER0_IRQ:

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
                   bullet_draw_list(bullet_list);
                   gunner_draw_list(shooter_list);

                   text_draw(in_game_timer->text);

                   sprite_set_pos(sp_crosshair, *get_mouse_X(), *get_mouse_Y());
                   sprite_draw(sp_crosshair);
                   graph_draw();

                   break;
               case KBC_IRQ:
                   if ((scancode[0]) == ESC_BREAK_CODE) {
                       good = false;
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
               case MOUSE_IRQ:
                   if (counter_mouse_ih >= 3) {
                       mouse_parse_packet(packet_mouse_ih, &pp);
                       update_mouse(&pp);
                       if (last_lb ^ keys->lb_pressed && keys->lb_pressed)
                           shoot_bullet(shooter1, bullet_list, bsp_bullet);
                       last_lb = keys->lb_pressed;
                       counter_mouse_ih = 0;

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

    timer_dtor(in_game_timer); in_game_timer = NULL;

    return SUCCESS;
}

#define ZOMBIES_NUM             5
#define ZOMBIE_HEALTH_FACTOR    1.1

static int (zombies)(void){

    int r;

    ent_set_scale(DEFAULT_SCALE);
    text_timer_t *in_game_timer = timer_ctor(consolas);

    list_t *shooter_list = list_ctor();

    gunner_t *shooter1 = gunner_ctor(bsp_shooter, bsp_pistol, gunner_player, 1); if(shooter1 == NULL) printf("Failed to get shooter1\n");
    gunner_set_spawn(shooter1, 75, 75);
    gunner_set_pos(shooter1, 75, 75);

    list_insert(shooter_list, list_end(shooter_list), shooter1);

    list_t *bullet_list  = list_ctor();

    ent_set_origin(gunner_get_x(shooter1)-ent_get_XLength()/2.0,
                   gunner_get_y(shooter1)-ent_get_YLength()/2.0);

   //uint32_t refresh_count_value = sys_hz() / REFRESH_RATE;
   uint8_t last_lb = 0;
   struct packet pp;
   keys_t *keys = get_key_presses();

    /// loop stuff
    uint32_t int_vector = 0;
    int good = true;
    int dead = false;

    int health = 50;

    while (good && !dead) {
        /* Get a request message. */
        if((r = get_interrupts_vector(&int_vector))) return r;
        for (uint32_t i = 0, n = 1; i < 32; i++, n <<= 1) {
            if (int_vector & n) {
               interrupt_handler(i);
               switch (i) {
               case TIMER0_IRQ: //printf("L463\n");

                   if (no_interrupts % 60 == 0) timer_update(in_game_timer);//printf("L465\n");
                   update_movement(map1, shooter1, keys, shooter_list);//printf("L466\n");

                   update_game_state(map1, shooter_list, bullet_list);//printf("L468\n");

                   if(list_find(shooter_list, shooter1) == list_end(shooter_list)){
                       good = false;
                       dead = true;
                       break;
                   } //printf("L489\n");

                   //update_scale();
                   double angle = get_mouse_angle(shooter1);//printf("L471\n");
                   gunner_set_angle(shooter1, angle - M_PI_2); //printf("L472\n");

                   ent_set_origin(gunner_get_x(shooter1)-ent_get_XLength()/2.0,
                                  gunner_get_y(shooter1)-ent_get_YLength()/2.0);

                   while(list_size(shooter_list) < ZOMBIES_NUM){
                       gunner_t *zombie = gunner_ctor(bsp_zombie, bsp_nothing, gunner_meelee, 3);
                       gunner_set_health(zombie, health);
                       gunner_set_curr_health(zombie, health);
                       health *= ZOMBIE_HEALTH_FACTOR;
                       get_random_spawn(map1, zombie);
                       list_push_back(shooter_list, zombie);
                   } //printf("L484\n");

                   graph_clear_screen();
                   map_draw   (map1);
                   bullet_draw_list(bullet_list);
                   gunner_draw_list(shooter_list);

                   text_draw(in_game_timer->text);

                   sprite_set_pos(sp_crosshair, *get_mouse_X(), *get_mouse_Y());
                   sprite_draw(sp_crosshair);
                   graph_draw(); //printf("L508\n");

                   break;
               case KBC_IRQ:
                   if ((scancode[0]) == ESC_BREAK_CODE) {
                       good = false;
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
               case MOUSE_IRQ:
                   if (counter_mouse_ih >= 3) {
                       mouse_parse_packet(packet_mouse_ih, &pp);
                       update_mouse(&pp);
                       if (last_lb ^ keys->lb_pressed && keys->lb_pressed)
                           shoot_bullet(shooter1, bullet_list, bsp_bullet);
                       last_lb = keys->lb_pressed;
                       counter_mouse_ih = 0;

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

    timer_dtor(in_game_timer); in_game_timer = NULL;

    return SUCCESS;
}

#define CHAT_MAX_SIZE   75
#define CHAT_MAX_NUM    19
text_t      *t_text[CHAT_MAX_NUM] = {NULL};
rectangle_t *r_text               =  NULL;
static void chat_process(const uint8_t *p, const size_t sz){
    char buffer2[CHAT_MAX_NUM+3];
    void *dest = NULL;
    hltp_type tp = hltp_interpret(p, sz, &dest);
    switch(tp){
        case hltp_type_string:
            strcpy(buffer2, dest);
            strncat(buffer2, " <", 2);
            for(size_t i = CHAT_MAX_NUM-1; i; --i)
                text_set_text(t_text[i], text_get_string(t_text[i-1]));
            text_set_text(t_text[0], buffer2);
            for(size_t i = 0; i < CHAT_MAX_NUM; ++i){
                if(text_get_string(t_text[i])[0] == '>'){
                    text_set_pos(t_text[i], rectangle_get_x(r_text)+50, text_get_y(t_text[i]));
                    text_set_halign(t_text[i], text_halign_left);
                }else{
                    text_set_pos(t_text[i], rectangle_get_x(r_text)+rectangle_get_w(r_text)-50, text_get_y(t_text[i]));
                    text_set_halign(t_text[i], text_halign_right);
                }
            }
            break;
        default: break;
    }
}
static int (chat)(void){
    int r;

    nctp_dump();
    nctp_set_processor(chat_process);

    struct packet pp;

    char buffer[CHAT_MAX_SIZE] = "";
    rectangle_t *r_buffer = NULL; {
        r_buffer = rectangle_ctor(0,0,900,70);
        rectangle_set_pos(r_buffer, graph_get_XRes()/2  -rectangle_get_w(r_buffer)/2,
                                    graph_get_YRes()*0.87-rectangle_get_h(r_buffer)/2);
        rectangle_set_fill_color   (r_buffer, GRAPH_BLACK);
        rectangle_set_outline_width(r_buffer, 2);
        rectangle_set_outline_color(r_buffer, GRAPH_WHITE);
        rectangle_set_fill_trans(r_buffer, GRAPH_TRANSPARENT);
    }
    text_t      *t_buffer = NULL; {
        t_buffer = text_ctor(consolas, "");
        text_set_pos(t_buffer, rectangle_get_x(r_buffer)+50,
                               rectangle_get_y(r_buffer)+rectangle_get_h(r_buffer)/2);
        text_set_halign(t_buffer, text_halign_left);
        text_set_valign(t_buffer, text_valign_center);
        text_set_color (t_buffer, TEXT_COLOR);
    }
    text_t      *t_size   = NULL; {
        t_size = text_ctor(consolas, "");
        text_set_pos(t_size, rectangle_get_x(r_buffer)+rectangle_get_w(r_buffer)-5,
                             rectangle_get_y(r_buffer)+rectangle_get_h(r_buffer)-5);
        text_set_halign(t_size, text_halign_right);
        text_set_valign(t_size, text_valign_bottom);
        text_set_color (t_size, TEXT_COLOR);
        text_set_size  (t_size, 18);
        char buffer2[20];
        sprintf(buffer2, "%d/%d", strlen(buffer), CHAT_MAX_SIZE);
        text_set_text(t_size, buffer2);
    }

    /** r_text */ {
        r_text = rectangle_ctor(0,0,900,550);
        rectangle_set_pos(r_text, graph_get_XRes()/2  -rectangle_get_w(r_buffer)/2,
                                  graph_get_YRes()*0.09);
        rectangle_set_fill_color   (r_text, GRAPH_BLACK);
        rectangle_set_outline_width(r_text, 2);
        rectangle_set_outline_color(r_text, GRAPH_WHITE);
        rectangle_set_fill_trans(r_text, GRAPH_TRANSPARENT);
    }
    /** t_text */ {
        for(size_t i = 0; i < CHAT_MAX_NUM; ++i){
            t_text[i] = text_ctor(consolas, " ");
            text_set_pos(t_text[i], rectangle_get_x(r_text)+50,
                                       rectangle_get_y(r_text)+rectangle_get_h(r_text)-30-25*i);
            text_set_halign(t_text[i], text_halign_left);
            text_set_valign(t_text[i], text_valign_bottom);
            text_set_color (t_text[i], TEXT_COLOR);
        }
    }

    /// loop stuff
    uint32_t int_vector = 0;
    int good = true;
    while (good) {
        /* Get a request message. */
        if((r = get_interrupts_vector(&int_vector))) return r;
        for (uint32_t i = 0, n = 1; i < 32; i++, n <<= 1) {
            if (int_vector & n) {
                interrupt_handler(i);
                switch (i) {
                case TIMER0_IRQ:
                    graph_clear_screen();
                    sprite_set_pos(sp_crosshair, *get_mouse_X(), *get_mouse_Y());

                    rectangle_draw(r_buffer);
                    text_draw(t_buffer);
                    text_draw(t_size);

                    rectangle_draw(r_text);
                    for(size_t i = 0; i < CHAT_MAX_NUM; ++i) text_draw(t_text[i]);

                    sprite_draw(sp_crosshair);
                    graph_draw();
                    break;
                case KBC_IRQ:
                    if      (scancode[0] == ESC_BREAK_CODE) good = false;
                    else if (scancode[0] == ENTER_MAKE_CODE) {
                        hltp_send_string(buffer);
                        char buffer2[CHAT_MAX_SIZE+3] = "> ";
                        strncat(buffer2, buffer, strlen(buffer));
                        for(size_t i = CHAT_MAX_NUM-1; i; --i)
                            text_set_text(t_text[i], text_get_string(t_text[i-1]));
                        text_set_text(t_text[0], buffer2);
                        for(size_t i = 0; i < CHAT_MAX_NUM; ++i){
                            if(text_get_string(t_text[i])[0] == '>'){
                                text_set_pos(t_text[i], rectangle_get_x(r_text)+50, text_get_y(t_text[i]));
                                text_set_halign(t_text[i], text_halign_left);
                            }else{
                                text_set_pos(t_text[i], rectangle_get_x(r_text)+rectangle_get_w(r_text)-50, text_get_y(t_text[i]));
                                text_set_halign(t_text[i], text_halign_right);
                            }
                        }
                        buffer[0] = '\0';
                    } else if(scancode[0] == BACKSPACE_MAKE_CODE){
                        buffer[strlen(buffer)-1] = '\0';
                    } else {
                        char c = map_makecode(scancode[0]);
                        if (c == ERROR_CODE) break;
                        if(strlen(buffer) < CHAT_MAX_SIZE) strncat(buffer, &c, 1);
                        else                               printf("Char limit exceeded\n");
                    }
                    text_set_text(t_buffer, buffer);
                    char buffer2[20];
                    sprintf(buffer2, "%d/%d", strlen(buffer), CHAT_MAX_SIZE);
                    text_set_text(t_size, buffer2);
                case MOUSE_IRQ:
                    if (counter_mouse_ih >= 3) {
                        mouse_parse_packet(packet_mouse_ih, &pp);
                        update_mouse(&pp);
                        counter_mouse_ih = 0;
                    }
                    break;
                case COM1_IRQ: nctp_ih(); break;
                }
            }
        }
    }

    rectangle_dtor(r_buffer);
    text_dtor     (t_buffer);

    rectangle_dtor(r_text);
    for(size_t i = 0; i < CHAT_MAX_NUM; ++i) text_dtor(t_text[i]);

    nctp_set_processor(NULL);

    return SUCCESS;
}

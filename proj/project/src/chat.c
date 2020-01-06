#include <lcom/lcf.h>

#include "chat.h"

#include "proj.h"

#include "proj_macros.h"
#include "proj_func.h"
#include "ent.h"
#include "interrupts_func.h"
#include "libs.h"
#include "makecode_map.h"
#include <math.h>

#include "hltp.h"

#define CHAT_MAX_SIZE   75
#define CHAT_MAX_NUM    19
static text_t      *t_text[CHAT_MAX_NUM] = {NULL};
static rectangle_t *r_text               =  NULL;

static void chat_process(const uint8_t *p, const size_t sz){
    char buffer2[CHAT_MAX_NUM+3];
    void *dest = NULL;
    hltp_type tp = hltp_interpret(p, sz, &dest);
    switch(tp){
        case hltp_type_string:
        strcpy(buffer2, dest);
        strncat(buffer2, " <", 2);
        for(size_t i = CHAT_MAX_NUM-1; i; --i)
        text_set_string(t_text[i], text_get_string(t_text[i-1]));
        text_set_string(t_text[0], buffer2);
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
        case hltp_type_invalid: break;
        case hltp_type_bullet : break;
        case hltp_type_host   : break;
        case hltp_type_remote : break;
    }
}

int (chat)(void){
    int r;

    nctp_dump();
    nctp_set_processor(chat_process);

    struct packet pp;

    char buffer[CHAT_MAX_SIZE] = "";
    rectangle_t *r_buffer = NULL; {
        r_buffer = rectangle_ctor(0,0,900,70);
        rectangle_set_pos(r_buffer, graph_get_XRes()/2  -rectangle_get_w(r_buffer)/2,
        (int16_t)(graph_get_YRes()*0.87-rectangle_get_h(r_buffer)/2));
        rectangle_set_fill_color   (r_buffer, GRAPH_BLACK);
        rectangle_set_outline_width(r_buffer, 2);
        rectangle_set_outline_color(r_buffer, GRAPH_WHITE);
        rectangle_set_fill_trans(r_buffer, GRAPH_TRANSPARENT);
    }
    text_t      *t_buffer = NULL; {
        t_buffer = text_ctor(font_get_default(), "");
        text_set_pos(t_buffer, rectangle_get_x(r_buffer)+50,
        rectangle_get_y(r_buffer)+rectangle_get_h(r_buffer)/2);
        text_set_halign(t_buffer, text_halign_left);
        text_set_valign(t_buffer, text_valign_center);
        text_set_color (t_buffer, TEXT_COLOR);
    }
    text_t      *t_size   = NULL; {
        t_size = text_ctor(font_get_default(), "");
        text_set_pos(t_size, rectangle_get_x(r_buffer)+rectangle_get_w(r_buffer)-5,
        rectangle_get_y(r_buffer)+rectangle_get_h(r_buffer)-5);
        text_set_halign(t_size, text_halign_right);
        text_set_valign(t_size, text_valign_bottom);
        text_set_color (t_size, TEXT_COLOR);
        text_set_size  (t_size, 18);
        char buffer2[20];
        sprintf(buffer2, "%d/%d", strlen(buffer), CHAT_MAX_SIZE);
        text_set_string(t_size, buffer2);
    }

    /** r_text */ {
    r_text = rectangle_ctor(0,0,900,550);
    rectangle_set_pos(r_text, graph_get_XRes()/2  -rectangle_get_w(r_buffer)/2,
    (int16_t)(graph_get_YRes()*0.09));
    rectangle_set_fill_color   (r_text, GRAPH_BLACK);
    rectangle_set_outline_width(r_text, 2);
    rectangle_set_outline_color(r_text, GRAPH_WHITE);
    rectangle_set_fill_trans(r_text, GRAPH_TRANSPARENT);
    }
    /** t_text */ {
    for(uint16_t i = 0; i < CHAT_MAX_NUM; ++i){
        t_text[i] = text_ctor(font_get_default(), " ");
        text_set_pos(t_text[i], rectangle_get_x(r_text)+50,
        rectangle_get_y(r_text)+rectangle_get_h(r_text)-30-25*i);
        text_set_halign(t_text[i], text_halign_left);
        text_set_valign(t_text[i], text_valign_bottom);
        text_set_color (t_text[i], TEXT_COLOR);
    }
}

/// loop stuff
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
                sprite_set_pos(sp_crosshair, *get_mouse_X(), *get_mouse_Y());

                rectangle_draw(r_buffer);
                text_draw(t_buffer);
                text_draw(t_size);

                rectangle_draw(r_text);
                for(size_t j = 0; j < CHAT_MAX_NUM; ++j) text_draw(t_text[j]);

                sprite_draw(sp_crosshair);
                graph_draw();
                break;
                case KBC_IRQ:
                if(!keyboard_get_done()) break;
                if      (keyboard_get_scancode()[0] == ESC_BREAK_CODE) good = false;
                else if (keyboard_get_scancode()[0] == ENTER_MAKE_CODE) {
                    hltp_send_string(buffer);
                    char buffer2[CHAT_MAX_SIZE+3] = "> ";
                    strncat(buffer2, buffer, strlen(buffer));
                    for(size_t j = CHAT_MAX_NUM-1; j; --j){
                        text_set_string(t_text[j], text_get_string(t_text[j-1]));
                    }
                    text_set_string(t_text[0], buffer2);
                    for(size_t j = 0; j < CHAT_MAX_NUM; ++j){
                        if(text_get_string(t_text[j])[0] == '>'){
                            text_set_pos(t_text[j], rectangle_get_x(r_text)+50, text_get_y(t_text[j]));
                            text_set_halign(t_text[j], text_halign_left);
                        }else{
                            text_set_pos(t_text[j], rectangle_get_x(r_text)+rectangle_get_w(r_text)-50, text_get_y(t_text[j]));
                            text_set_halign(t_text[j], text_halign_right);
                        }
                    }
                    buffer[0] = '\0';
                } else if(keyboard_get_scancode()[0] == BACKSPACE_MAKE_CODE){
                    buffer[strlen(buffer)-1] = '\0';
                } else {
                    char c = map_makecode(keyboard_get_scancode()[0]);
                    if (c == ERROR_CODE) break;
                    if(strlen(buffer) < CHAT_MAX_SIZE) strncat(buffer, &c, 1);
                    else                               printf("Char limit exceeded\n");
                }
                text_set_string(t_buffer, buffer);
                char buffer2[20];
                sprintf(buffer2, "%d/%d", strlen(buffer), CHAT_MAX_SIZE);
                text_set_string(t_size, buffer2);
                case MOUSE_IRQ:
                if (mouse_get_counter_mouse_ih() >= 3) {
                    mouse_parse_packet(mouse_get_packet_mouse_ih(), &pp);
                    update_mouse(&pp);
                    mouse_set_counter_mouse_ih(0);
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

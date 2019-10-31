#include <lcom/lcf.h>

#include "state_machine.h"
#include "mouse_macros.h"
#include "mouse.h"
#include "errors.h"
#include "utils.h"

struct mouse_ev* mouse_get_event(struct packet *pp) {

    static struct mouse_ev event;
    static struct packet last; // compare consecutive events
    static int aux = 1; // first iteration

    if (pp == NULL)
        return &event;

    // current packet
    uint8_t lb_press = pp->bytes[0] & LEFT_BUTTON;
    uint8_t rb_press = pp->bytes[0] & RIGHT_BUTTON;
    uint8_t mb_press = pp->bytes[0] & MIDDLE_BUTTON;
    int16_t delta_x = pp->delta_x;
    int16_t delta_y = pp->delta_y;

    if (aux) { // first event detected
        if (lb_press && (rb_press | mb_press) == 0 && delta_x == 0 && delta_y == 0)
            event.type = LB_PRESSED;
        else if (rb_press && (lb_press | mb_press) == 0 && delta_x == 0 && delta_y == 0)
            event.type = RB_PRESSED;
        else if ((delta_x || delta_y) && (lb_press | rb_press | mb_press) == 0) {
            event.type = MOUSE_MOV;
            event.delta_x = delta_x;
            event.delta_y = delta_y;
        } else
            event.type = BUTTON_EV;
        aux = 0;
    } else {
        // last packet
        uint8_t last_lb_press = last.bytes[0] & LEFT_BUTTON;
        uint8_t last_rb_press = last.bytes[0] & RIGHT_BUTTON;
        uint8_t last_mb_press = last.bytes[0] & MIDDLE_BUTTON;

        if (lb_press && (rb_press | mb_press) == 0 && delta_x == 0 && delta_y == 0 && last_lb_press == 0)
            event.type = LB_PRESSED;
        else if (rb_press && (lb_press | mb_press) == 0 && delta_x == 0 && delta_y == 0 && last_rb_press == 0)
            event.type = RB_PRESSED;
        else if ((lb_press | rb_press | mb_press) == 0 && delta_x == 0 && delta_y == 0 && last_lb_press)
            event.type = LB_RELEASED;
        else if ((lb_press | rb_press | mb_press) == 0 && delta_x == 0 && delta_y == 0 && last_rb_press)
            event.type = RB_RELEASED;
        else if ((delta_x || delta_y) && (lb_press | rb_press | mb_press) == (last_lb_press | last_rb_press | last_mb_press)) {
            event.type = MOUSE_MOV;
            event.delta_x = delta_x;
            event.delta_y = delta_y;
        } else
            event.type = BUTTON_EV;
    }

    // update last packet for comparison
    last = *pp;

    return &event;
}

int state_machine(struct mouse_ev* event, uint8_t x_len, uint8_t tolerance) {

    static enum states state = INITIAL;
    static int response = OTHER_ERROR;
    static int x_length = 0;
    static int y_length = 0;

    if (event == NULL)
        return response;
    switch (state) {
        case INITIAL:
            if (event->type == LB_PRESSED)
                state = DRAG_UP;
            break;
        case DRAG_UP:
            if (event->type == MOUSE_MOV) {
                if (abs(event->delta_x) > tolerance || abs(event->delta_y) > tolerance) {
                    state = INITIAL;
                    x_length = 0;
                    y_length = 0;
                    break;
                }

                x_length += event->delta_x;
                y_length += event->delta_y;
            } else if (event->type == LB_RELEASED) {
                if (x_length == 0 || y_length == 0) {
                    state = INITIAL;
                    break;
                }

                int slope = y_length / x_length;

                if (slope <= 1 || x_length < x_len) {
                    state = INITIAL;
                    x_length = 0;
                    y_length = 0;
                    break;
                }

                state = VERTEX;
                x_length = 0;
                y_length = 0;
            } else {
                state = INITIAL;
                x_length = 0;
                y_length = 0;
            }
            break;
        case VERTEX:
            if (event->type == MOUSE_MOV) {
                if (abs(event->delta_x) > tolerance || abs(event->delta_y) > tolerance)
                    state = INITIAL;
            } else if (event->type == RB_PRESSED) {
                state = DRAG_DOWN;
            } else if (event->type == LB_PRESSED) {
                state = DRAG_UP;
            } else
                state = INITIAL;
            break;
        case DRAG_DOWN:
            if (event->type == MOUSE_MOV) {
                if (abs(event->delta_x) > tolerance || abs(event->delta_y) > tolerance) {
                    state = INITIAL;
                    x_length = 0;
                    y_length = 0;
                    break;
                }

                x_length += event->delta_x;
                y_length += event->delta_y;
            } else if (event->type == RB_RELEASED) {
                if (x_length == 0 || y_length == 0) {
                    state = INITIAL;
                    break;
                }
                int slope = y_length / x_length;

                if (slope >= -1 || x_length < x_len) {
                    state = INITIAL;
                    x_length = 0;
                    y_length = 0;
                    break;
                }

                state = FINAL;
                response = SUCCESS;
                x_length = 0;
                y_length = 0;
            } else {
                state = INITIAL;
                x_length = 0;
                y_length = 0;
            }
            break;
        case FINAL: // acception state
            response = SUCCESS;
        default: // invalid state / dead state
            response = INVALID_STATE;
    }
    return response;
}

#include <lcom/lcf.h>

#include "mouse.h"

#include "errors.h"
#include "kbc_macros.h"
#include "mouse_macros.h"
#include "kbc.h"

int (subscribe_mouse_interrupt)(uint8_t interrupt_bit, int *interrupt_id) {
    if (interrupt_id == NULL) return NULL_PTR;
    *interrupt_id = interrupt_bit;
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, interrupt_id)) return SBCR_ERROR;
    return SUCCESS;
}

int got_error_mouse_ih = 0;
uint8_t packet[3];
int counter = 0;

void (mouse_ih)(void) {
    uint8_t status = 0;
    got_error_mouse_ih = 0;
    if(counter >= 3) counter = 0;

    if (util_sys_inb(STATUS_REG, &status)) {
        got_error_mouse_ih = 1;
        return;
    }

    if (status & (TIME_OUT_REC | PARITY_ERROR)) {
        got_error_mouse_ih = 1;
        return;
    }

    uint8_t byte = 0;

    if (util_sys_inb(OUTPUT_BUF, &byte)) {
        got_error_mouse_ih = 1;
        return;
    }

    /// This does not run if: I was expecting the first one but what I get is definitely not the first byte
    if((byte & FIRST_BYTE_ID)  || counter){
        packet[counter] = byte;
        counter++;
    }
}

struct packet (mouse_parse_packet)(const uint8_t *packet_bytes){
    struct packet pp;
    pp.bytes[0] = packet_bytes[0];
    pp.bytes[1] = packet_bytes[1];
    pp.bytes[2] = packet_bytes[2];
    pp.rb       = pp.bytes[0] & RIGHT_BUTTON;
    pp.mb       = pp.bytes[0] & MIDDLE_BUTTON;
    pp.lb       = pp.bytes[0] & LEFT_BUTTON;
    pp.delta_x  = pp.bytes[1];
    pp.delta_y  = pp.bytes[2];
    pp.x_ov     = pp.bytes[0] & X_OVERFLOW;
    pp.y_ov     = pp.bytes[0] & Y_OVERFLOW;
    return pp;
}

int (mouse_set_data_report)(int on){
    int ret = 0;
    if((ret = kbc_issue_cmd(MOUSE_WRITE_B))) return ret;
    if(sys_outb(KBC_CMD_ARG, DIS_DATA_REP)) return WRITE_ERROR;
    return SUCCESS;
}

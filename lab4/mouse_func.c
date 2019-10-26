#include "mouse_func.h"
#include "errors.h"
#include "kbc.h"
#include "mouse.h"

#include <lcom/lcf.h>

int (subscribe_mouse_interrupt)(uint8_t interrupt_bit, int *interrupt_id) {
    if (interrupt_id == NULL) return NULL_PTR;
    *interrupt_id = interrupt_bit;
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, interrupt_id)) return SBCR_ERROR;
    return SUCCESS;
}

int (unsubscribe_interrupt)(int *interrupt_id) {
    if (interrupt_id == NULL) return NULL_PTR;
    if (sys_irqrmpolicy(interrupt_id)) return UNSBCR_ERROR;
    return SUCCESS;
}

int got_error = 0;
uint8_t packet[3];
int counter = 0;

void (mouse_ih)(void) {
    uint8_t status = 0;
    got_error = 0;

    if (util_sys_inb(STATUS_REG, &status)) {
        got_error = 1;
        return;
    }

    if (status & (TIME_OUT_REC | PARITY_ERROR)) {
        got_error = 1;
        return;
    }

    uint8_t byte = 0;

    if (util_sys_inb(OUTPUT_BUF, &byte)) {
        got_error = 1;
        return;
    }

    if(counter >= 3) counter = 0;
    packet[counter] = byte;
    counter++;

}

struct packet (mouse_parse_packet)(const uint8_t *packet_bytes){
    struct packet pp;
    pp.bytes[0] = packet_bytes[0];
    pp.bytes[1] = packet_bytes[1];
    pp.bytes[2] = packet_bytes[2];
    pp.rb       = pp.bytes[0] & RIGHT_BUTTON;
    pp.mb       = pp.bytes[0] & MIDDLE_BUTTON;
    pp.lb       = pp.bytes[0] & LEFT_BUTTON;
    //pp.delta_x  = 
    return pp;
}

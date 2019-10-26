#include <lcom/lcf.h>

#include "kbd.h"

#include "kbc_macros.h"
#include "kbc.h"

int (subscribe_kbc_interrupt)(uint8_t interrupt_bit, int *interrupt_id) {
    if (interrupt_id == NULL) return 1;
    *interrupt_id = interrupt_bit;
    return (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, interrupt_id));
}

void (kbc_ih)(void) {
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

    if (two_byte_scancode) {
        scancode[1] = byte;
        two_byte_scancode = 0;
    } else {
        scancode[0] = byte;
        two_byte_scancode = (byte == TWO_BYTE_CODE);
    }

}

int (kbd_poll)(uint8_t bytes[], uint8_t *size){
    if(bytes == NULL || size == NULL) return 1;
    uint8_t c;
    if(kbc_read_byte(&c)) return 1;
    if(c == TWO_BYTE_CODE){
        if(kbc_read_byte(&bytes[1])) return 1;
        bytes[0] = c;
        *size = 2;
    }else{
        bytes[1] = 0;
        bytes[0] = c;
        *size = 1;
    }
    return 0;
}

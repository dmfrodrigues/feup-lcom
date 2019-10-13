#include <lcom/lcf.h>

#include "kbc.h"
#include "kbc_func.h"

int (subscribe_kbc_interrupt)(uint8_t interrupt_bit, int *interrupt_id) {
    if (interrupt_id == NULL) return 1;
    *interrupt_id = interrupt_bit;
    return (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, interrupt_id));
}

int (unsubscribe_kbc_interrupt)(int *interrupt_id) {
    if (interrupt_id == NULL) return 1;
    return sys_irqrmpolicy(interrupt_id);
}

uint8_t scancode[2];
int two_byte_scancode = 0;

void (kbc_ih)(void) {
    uint8_t status = 0;

    #ifdef LAB3
    sys_inb_counter(INCREMENT);
    #endif

    if (util_sys_inb(STATUS_REG, &status)) return;

    if (status & (TIME_OUT_REC | PARITY_ERROR)) return;

    #ifdef LAB3
    sys_inb_counter(INCREMENT);
    #endif

    uint8_t byte = 0;

    if (util_sys_inb(OUTPUT_BUF, &byte)) return;

    if (two_byte_scancode) {
        scancode[1] = byte;
        two_byte_scancode = 0;
    } else {
        scancode[0] = byte;
        two_byte_scancode = (byte == TWO_BYTE_CODE);
    }

}

uint32_t sys_inb_counter(int increment) {
    static uint32_t counter = 0;
    if (increment) return ++counter;
    return counter;
}

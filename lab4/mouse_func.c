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

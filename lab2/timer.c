#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
    /* To be implemented by the students */
    printf("%s is not yet implemented!\n", __func__);

    return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
    printf("%s is not yet implemented!\n", __func__);

    return 1;
}

int (timer_unsubscribe_int)() {
    /* To be implemented by the students */
    printf("%s is not yet implemented!\n", __func__);

    return 1;
}

void (timer_int_handler)() {
    /* To be implemented by the students */
    printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
    int write_port = TIMER_CTRL;
    u32_t cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
    if(sys_outb(write_port, cmd)) return 1;

    int read_port;
    switch(timer){
        case 0: read_port = TIMER_0; break;
        case 1: read_port = TIMER_1; break;
        case 2: read_port = TIMER_2; break;
        default: return 1;      break;
    }
    if(util_sys_inb(read_port, st)) return 1;
    return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
    enum timer_status_field field) {
        /* To be implemented by the students */
        printf("%s is not yet implemented!\n", __func__);

        return 1;
    }

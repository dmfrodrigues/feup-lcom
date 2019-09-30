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
    switch(timer) {
        case 0: read_port = TIMER_0; break;
        case 1: read_port = TIMER_1; break;
        case 2: read_port = TIMER_2; break;
        default: return 1;           break;
    }
    if(util_sys_inb(read_port, st)) return 1;
    return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
    union timer_status_field_val conf;
    uint8_t in_mode;
    if(field == tsf_all || field == tsf_initial){
        in_mode = (st & TIMER_INMODE_MASK) >> TIMER_INMODE_POS;
        switch(in_mode){
            case 0: conf.in_mode = INVAL_val    ; break;
            case 1: conf.in_mode = LSB_only     ; break;
            case 2: conf.in_mode = MSB_only     ; break;
            case 3: conf.in_mode = MSB_after_LSB; break;
            default: return 1; break;
        }
        if(timer_print_config(timer, field, conf)) return 1;
    }
    if(field == tsf_mode){
        conf.count_mode = (st & TIMER_MODE_MASK)>>TIMER_MODE_POS;
        if(timer_print_config(timer, field, conf)) return 1;
    }
    if(field == tsf_base){
        conf.bcd = st & TIMER_BCD;
        if(timer_print_config(timer, field, conf)) return 1;
    }
    return 0;
}

#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

    // Frequencies out this range are not supported (by limitation of hardware)
    if (freq > TIMER_FREQ || freq < TIMER_MIN_FREQ) return 1;

    uint8_t status = 0;
    if (timer_get_conf(timer, &status)) return 1;
    //Make command
    uint8_t write_cmd = 0;
    //Select timer
    switch(timer) {
        case 0: write_cmd |= TIMER_SEL0; break;
        case 1: write_cmd |= TIMER_SEL1; break;
        case 2: write_cmd |= TIMER_SEL2; break;
        default: return 1;               break;
    }
    //Change both LSB and MSB
    write_cmd |= TIMER_LSB_MSB;
    //Keep 4 least significant bits
    write_cmd |= (status & (TIMER_MODE_MASK | TIMER_BCD));
    //Write cmd
    if (sys_outb(TIMER_CTRL, write_cmd)) return 1;

    // counter_init = clock/freq
    uint16_t counter_init = (uint16_t)(TIMER_FREQ / freq);

    int timer_port = 0;
    switch(timer) {
        case 0: timer_port = TIMER_0; break;
        case 1: timer_port = TIMER_1; break;
        case 2: timer_port = TIMER_2; break;
        default: return 1;            break;
    }
    uint8_t lsb = 0, msb = 0;
    if (util_get_LSB(counter_init, &lsb)) return 1;
    if (util_get_MSB(counter_init, &msb)) return 1;

    if (sys_outb(timer_port, lsb)) return 1;
    if (sys_outb(timer_port, msb)) return 1;

    return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    int hook_id = 2;
    if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)) return 1;
    *bit_no = hook_id;
    return 0;
}

int (timer_unsubscribe_int)(uint8_t *bit_no) {
    int hook_id = *bit_no;
    if(sys_irqrmpolicy(&hook_id)) return 1;
    *bit_no = hook_id;
    return 0;
}

void (timer_int_handler)() {
    int no_interrupts = 0; //should be the global

    no_interrupts++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
    // Write read-back command to TIMER_CTRL
    u32_t cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
    if(sys_outb(TIMER_CTRL, cmd)) return 1;

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
    switch(field){
        case tsf_all:
            conf.byte = st;
            break;
        case tsf_initial:
            in_mode = (st & TIMER_INMODE_MASK) >> TIMER_INMODE_POS;
            switch(in_mode){
                case 0: conf.in_mode = INVAL_val    ; break; //000
                case 1: conf.in_mode = LSB_only     ; break; //001
                case 2: conf.in_mode = MSB_only     ; break; //010
                case 3: conf.in_mode = MSB_after_LSB; break; //011
                default: return 1; break;
            }
            break;
        case tsf_mode:
            conf.count_mode = (st & TIMER_MODE_MASK)>>TIMER_MODE_POS;
            if(conf.count_mode == TIMER_MODE_2ALT || conf.count_mode == TIMER_MODE_3ALT)
                conf.count_mode &= TIMER_MODE_RED2;
            break;
        case tsf_base:
            conf.bcd = st & TIMER_BCD;
            break;
        default: return 1; break;
    }
    if(timer_print_config(timer, field, conf)) return 1;
    return 0;
}

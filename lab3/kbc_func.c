#include "kbc_func.h"
#include "kbc.h"

#include <lcom/lcf.h>

int (subscribe_kbc_interrupt)(uint8_t interrupt_bit, int *interrupt_id) {
    if (interrupt_id == NULL) return 1;
    *interrupt_id = interrupt_bit;
    return (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, interrupt_id));
}

int (unsubscribe_interrupt)(int *interrupt_id) {
    if (interrupt_id == NULL) return 1;
    return sys_irqrmpolicy(interrupt_id);
}

uint8_t scancode[2];
int got_error = 0;
int done = 1;
int sz = 1;

void (kbc_ih)(void) {

    if(done) sz = 1;
    else     sz++;

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

    if ((status & OUT_BUF_FUL) == 0 || (status & AUX_MOUSE) != 0) {
        got_error = 1;
        return;
    }

    uint8_t byte = 0;

    if (util_sys_inb(OUTPUT_BUF, &byte)) {
        got_error = 1;
        return;
    }

    scancode[sz-1] = byte;
    done = !(TWO_BYTE_CODE == byte);

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

int (kbc_read_cmd)(uint8_t *cmd){
    if(kbc_issue_cmd(READ_KBC_CMD)) return 1;
    if(kbc_read_byte(cmd)) return 1;
    return 0;
}

int (kbc_change_cmd)(uint8_t cmd){
    if(kbc_issue_cmd(WRITE_KBC_CMD)) return 1;
    if(kbc_issue_arg(cmd)) return 1;
    return 0;
}

int (kbc_restore_kbd)(){
    uint8_t cmd = 0;
    if(kbc_read_cmd(&cmd)) return 1;
    cmd = (cmd | INT_KBD) & (~DIS_KBD);
    if(kbc_change_cmd(cmd)) return 1;
    return 0;
}

int (kbc_issue_cmd)(uint8_t cmd){
    uint8_t stat;
    for(int i = 0; i < KBC_NUM_TRIES; ++i){
        if(util_sys_inb(STATUS_REG, &stat)) return 1;
        if((stat&IN_BUF_FULL) == 0){
            if(sys_outb(KBC_CMD, cmd)) return 1;
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY));
    }
    return 1;
}

int (kbc_issue_arg)(uint8_t arg){
    uint8_t stat;
    for(int i = 0; i < KBC_NUM_TRIES; ++i){
        if(util_sys_inb(STATUS_REG, &stat)) return 1;
        if((stat&IN_BUF_FULL) == 0){
            if(sys_outb(KBC_CMD_ARG, arg)) return 1;
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY));
    }
    return 1;
}

int (kbc_read_byte)(uint8_t *byte){
    uint8_t stat;
    while(true){
        if(util_sys_inb(STATUS_REG, &stat)) return 1;
        if((stat&OUT_BUF_FUL) && (stat&AUX_MOUSE)==0){
            if(stat & (PARITY_ERROR | TIME_OUT_REC)) return 1;
            if(util_sys_inb(OUTPUT_BUF, byte)) return 1;
            else return 0;
        }
        tickdelay(micros_to_ticks(DELAY));
    }
}

#include <lcom/lcf.h>

#include "kbc.h"

#include "kbc_macros.h"
#include "utils.h"

int (unsubscribe_interrupt)(int *interrupt_id) {
    if (interrupt_id == NULL) return 1;
    return sys_irqrmpolicy(interrupt_id);
}

int (kbc_read_cmd)(uint8_t *cmd){
    if(kbc_issue_cmd(READ_KBC_CMD)) return 1;
    if(kbc_read_byte(cmd)) return 1;
    return 0;
}

int (kbc_change_cmd)(uint8_t cmd){
    if(kbc_issue_cmd(WRITE_KBC_CMD)) return 1;
    if(sys_outb(KBC_CMD_ARG, cmd)) return 1;
    return 0;
}

int (kbc_restore_keyboard)(){
    uint8_t cmd = 0;
    if(kbc_read_cmd(&cmd)) return 1;
    cmd = (cmd | INT_KBD) & (~DIS_KBD) ;
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

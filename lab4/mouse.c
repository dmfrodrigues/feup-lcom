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

    if ((got_error_mouse_ih = util_sys_inb(STATUS_REG, &status))) return;

    if (status & (TIME_OUT_REC | PARITY_ERROR)) {
        got_error_mouse_ih = OTHER_ERROR;
        return;
    }

    uint8_t byte = 0;

    if ((got_error_mouse_ih = util_sys_inb(OUTPUT_BUF, &byte))) return;

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
    printf("L61\n");
    if((ret = kbc_issue_cmd(MOUSE_WRITE_B))) return ret; printf("L62\n");
    if((ret = kbc_issue_arg(DIS_DATA_REP))) return ret; printf("L63\n");
    uint8_t ack = 0;
    if((ret = kbc_read_byte(&ack))) return ret;printf("L65\n");
    printf("ACK: %x\n", ack);
    return SUCCESS;
}

int (mouse_read_data)(uint8_t *data) {
    int ret;
    if ((ret = mouse_issue_cmd(READ_DATA))) return ret;
    if ((ret = mouse_read_byte(data))) return ret;
    return SUCCESS;
}

int (mouse_issue_cmd)(uint32_t cmd) {
    int ret;
    uint8_t ack = 0;
    while (ack != ACK_ERROR) {
        if ((ret = kbc_issue_cmd(MOUSE_WRITE_B))) return ret;
        if ((ret = kbc_issue_arg(cmd))) return ret;
        if ((ret = mouse_read_byte(&ack))) return ret;
        if (ack == ACK_OK) return SUCCESS;
        tickdelay(micros_to_ticks(DELAY));
    }
    return INVALID_COMMAND;
}

int (mouse_read_byte)(uint8_t *byte) {
    int ret = 0;
    uint8_t stat;
    for(int i = 0; i < KBC_NUM_TRIES; ++i){
        if((ret = util_sys_inb(STATUS_REG, &stat))) return ret;
        if((stat&OUT_BUF_FUL) && (stat&AUX_MOUSE)) {
            if(stat & (PARITY_ERROR | TIME_OUT_REC)) return OTHER_ERROR;
            if((ret = util_sys_inb(OUTPUT_BUF, byte))) return ret;
            else return SUCCESS;
        }
        tickdelay(micros_to_ticks(DELAY));
    }
    return TIMEOUT_ERROR;
}

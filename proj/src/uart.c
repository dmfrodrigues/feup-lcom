#include <lcom/lcf.h>

#include "uart.h"

#include "errors.h"

#define UART_RBR            0
#define UART_THR            0
#define UART_IER            1
#define UART_IIR            2
#define UART_FCR            2
#define UART_LCR            3
#define UART_MCR            4
#define UART_LSR            5
#define UART_MSR            6
#define UART_SR             7

#define UART_DLL            0
#define UART_DLM            1

#define UART_BITS_PER_CHAR_POS 0
#define UART_STOP_BITS_POS     2
#define UART_PARITY_POS        3
#define UART_BREAK_CONTROL_POS 6
#define UART_DLAB_POS          7

#define UART_BITS_PER_CHAR  (BIT(0) | BIT(1))
#define UART_STOP_BITS      (BIT(2))
#define UART_PARITY         (BIT(3) | BIT(4) | BIT(5))
#define UART_BREAK_CONTROL  (BIT(6))
#define UART_DLAB           (BIT(7))

#define UART_GET_BITS_PER_CHAR(n) (((n)&UART_BITS_PER_CHAR) + 5)
#define UART_GET_STOP_BITS(n)     (((n)&UART_STOP_BITS)? 2 : 1)
#define UART_GET_PARITY(n)        (((n)&UART_PARITY       )>>UART_PARITY_POS       )
#define UART_GET_BREAK_CONTROL(n) (((n)&UART_BREAK_CONTROL)>>UART_BREAK_CONTROL_POS)
#define UART_GET_DLAB(n)          (((n)&UART_DLAB         )>>UART_DLAB_POS         )
#define UART_GET_DIV_LATCH(m,l)   ((m)<<8 | (l))
#define UART_GET_DLL(n)           ((n)&0xFF)
#define UART_GET_DLM(n)           (((n)>>8)&0xFF)

#define UART_RECEIVED_DATA_POS      0
#define UART_TRANSMITTER_EMPTY_POS  1
#define UART_RECEIVER_LINE_STAT_POS 2
#define UART_MODEM_STAT_POS         3

#define UART_RECEIVED_DATA          (BIT(0))
#define UART_TRANSMITTER_EMPTY      (BIT(1))
#define UART_RECEIVER_LINE_STAT     (BIT(2))
#define UART_MODEM_STAT             (BIT(3))

#define UART_GET_RECEIVED_DATA(n)       (((n)&UART_RECEIVED_DATA     )>>UART_RECEIVED_DATA_POS     )
#define UART_GET_TRANSMITTER_EMPTY(n)   (((n)&UART_TRANSMITTER_EMPTY )>>UART_TRANSMITTER_EMPTY_POS )
#define UART_GET_RECEIVER_LINE_STAT(n)  (((n)&UART_RECEIVER_LINE_STAT)>>UART_RECEIVER_LINE_STAT_POS)
#define UART_GET_MODEM_STAT(n)          (((n)&UART_MODEM_STAT        )>>UART_MODEM_STAT_POS        )

int uart_get_config(int base_addr, uart_config *config){
    int ret = SUCCESS;

    config->base_addr = base_addr;

    if((ret = util_sys_inb(base_addr+UART_LCR, &config->lcr))) return ret;

    if((ret = util_sys_inb(base_addr+UART_IER, &config->ier))) return ret;

    if((ret = uart_enable_divisor_latch (base_addr))) return ret;
    if((ret = util_sys_inb(base_addr+UART_DLL, &config->dll   ))) return ret;
    if((ret = util_sys_inb(base_addr+UART_DLM, &config->dlm   ))) return ret;
    if((ret = uart_disable_divisor_latch(base_addr))) return ret;

    uart_parse_config(config);
    return ret;
}
void uart_parse_config(uart_config *config){
    /// LCR
    config->bits_per_char          = UART_GET_BITS_PER_CHAR     (config->lcr);
    config->stop_bits              = UART_GET_STOP_BITS         (config->lcr);
    config->parity                 = UART_GET_PARITY            (config->lcr);
    config->break_control          = UART_GET_BREAK_CONTROL     (config->lcr);
    config->dlab                   = UART_GET_DLAB              (config->lcr);
    /// IER
    config->received_data_int      = UART_GET_RECEIVED_DATA     (config->ier);
    config->transmitter_empty_int  = UART_GET_TRANSMITTER_EMPTY (config->ier);
    config->receiver_line_stat_int = UART_GET_RECEIVER_LINE_STAT(config->ier);
    config->modem_stat_int         = UART_GET_MODEM_STAT        (config->ier);
    /// DIV LATCH
    config->divisor_latch          = UART_GET_DIV_LATCH(config->dlm, config->dll);
}
void uart_print_config(uart_config config){
    printf("Base address                                   : 0x%X\n", config.base_addr);
    printf("    LCR configuration                          : 0x%02X\n", config.lcr);
    printf("        Number of bits per char                : %d\n", config.bits_per_char);
    printf("        Number of stop bits                    : %d\n", config.stop_bits);
    printf("        Parity                                 : ");
    if((config.parity&1) == 0) printf("no parity\n");
    else switch(config.parity){
        case uart_parity_odd : printf("odd parity\n"     ); break;
        case uart_parity_even: printf("even parity\n"    ); break;
        case uart_parity_par1: printf("parity bit is 1\n"); break;
        case uart_parity_par0: printf("parity bit is 0\n"); break;
        default              : printf("invalid\n"        ); break;
    }
    printf("    Break control                              : %d\n", config.break_control);
    printf("    DLAB                                       : %d\n", config.dlab);

    printf("    IER configuration                          : 0x%02X\n", config.ier);
    printf("        Received data interrupts enabled       : %d\n", config.received_data_int);
    printf("        Transmitter empty interrupts enabled   : %d\n", config.transmitter_empty_int);
    printf("        Receiver line status interrupts enabled: %d\n", config.receiver_line_stat_int);
    printf("        Modem status interrupts enabled        : %d\n", config.modem_stat_int);

    printf("    Divisor latch (DLM & DLL)                  : %d\n", config.divisor_latch);
    printf("        Bit rate                               : %d\n", UART_BITRATE/config.divisor_latch);
}

int uart_enable_divisor_latch(int base_addr){
    int ret = SUCCESS;
    uint8_t conf; if((ret = util_sys_inb(base_addr+UART_LCR, &conf))) return ret;
    return uart_write_config(base_addr, conf | UART_DLAB);
}
int uart_disable_divisor_latch(int base_addr){
    int ret = SUCCESS;
    uint8_t conf; if((ret = util_sys_inb(base_addr+UART_LCR, &conf))) return ret;
    return uart_write_config(base_addr, conf & (~UART_DLAB));
}

int uart_write_config(int base_addr, uint8_t config){
    if(sys_outb(base_addr+UART_LCR, config)) return WRITE_ERROR;
    return SUCCESS;
}
int uart_set_bits_per_character(int base_addr, uint8_t bits_per_char){
    if(bits_per_char < 5 || bits_per_char > 8) return INVALID_ARG;
    int ret = SUCCESS;
    bits_per_char = (bits_per_char-5)&0x3;
    uint8_t conf; if((ret = util_sys_inb(base_addr+UART_LCR, &conf))) return ret;
    conf = (conf & (~UART_BITS_PER_CHAR)) | bits_per_char;
    return uart_write_config(base_addr, conf);
}
int uart_set_stop_bits(int base_addr, uint8_t stop){
    if(stop != 1 && stop != 2) return INVALID_ARG;
    int ret = SUCCESS;
    stop -= 1;
    stop = (stop&1)<<2;
    uint8_t conf; if((ret = util_sys_inb(base_addr+UART_LCR, &conf))) return ret;
    conf = (conf & (~UART_STOP_BITS)) | stop;
    return uart_write_config(base_addr, conf);
}
int uart_set_parity(int base_addr, uart_parity par){
    int ret = SUCCESS;
    uint8_t parity = par << 3;
    uint8_t conf; if((ret = util_sys_inb(base_addr+UART_LCR, &conf))) return ret;
    conf = (conf & (~UART_PARITY)) | parity;
    return uart_write_config(base_addr, conf);
}
int uart_set_bit_rate(int base_addr, float bit_rate){
    int ret = SUCCESS;
    uint16_t latch = UART_BITRATE/bit_rate;
    uint8_t dll = UART_GET_DLL(latch);
    uint8_t dlm = UART_GET_DLM(latch);
    if((ret = uart_enable_divisor_latch(base_addr))) return ret;
    if(sys_outb(base_addr+UART_DLL, dll)) return WRITE_ERROR;
    if(sys_outb(base_addr+UART_DLM, dlm)) return WRITE_ERROR;
    if((ret = util_sys_inb(base_addr+UART_DLM, &dlm))) return ret;
    return SUCCESS;
}

#include <lcom/lcf.h>

#include "uart.h"

#include "errors.h"

int uart_get_config(int base_addr, uart_config *config){
    int ret = SUCCESS;

    config->base_addr = base_addr;

    if((ret = util_sys_inb(base_addr+UART_LCR, &config->config))) return ret;

    if((ret = uart_enable_divisor_latch(base_addr))) return ret;
    if((ret = util_sys_inb(base_addr+UART_DLL, &config->dll   ))) return ret;
    if((ret = util_sys_inb(base_addr+UART_DLM, &config->dlm   ))) return ret;

    uart_parse_config(config);
    return ret;
}
void uart_parse_config(uart_config *config){
    config->bits_per_char        = UART_GET_BITS_PER_CHAR(config->config);
    config->stop_bits            = UART_GET_STOP_BITS    (config->config);
    config->parity               = UART_GET_PARITY       (config->config);
    config->break_control        = UART_GET_BREAK_CONTROL(config->config);
    config->divisor_latch        = config->dlm<<8 | config->dll;
}
void uart_print_config(uart_config config){
    printf("Base address: 0x%X\n", config.base_addr);
    printf("Configuration: 0x%02X\n", config.config);
    printf("Number of bits per char: %d\n", config.bits_per_char);
    printf("Number of stop bits: %d\n", config.stop_bits);
    printf("Parity: ");
    if((config.parity&1) == 0) printf("no parity\n");
    else switch(config.parity){
        case uart_parity_odd : printf("odd parity\n"     ); break;
        case uart_parity_even: printf("even parity\n"    ); break;
        case uart_parity_par1: printf("parity bit is 1\n"); break;
        case uart_parity_par0: printf("parity bit is 0\n"); break;
        default              : printf("invalid\n"        ); break;
    }
    printf("Break control: %d\n", config.break_control);
    printf("Divisor latch: %d\n", config.divisor_latch);
    printf("Bit rate (x1000): %d\n", 1000*(uint32_t)UART_BITRATE/config.divisor_latch);
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
    printf("WRITING CONFIG 0x%02X TO 0x%X\n", config, base_addr);
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
int uart_set_bit_rate(int base_addr, float bit_rate){ printf("%s, L82\n", __func__);
    int ret = SUCCESS;
    uint16_t latch = UART_BITRATE/bit_rate;
    uint8_t dll = latch&0xFF;
    uint8_t dlm = (latch>>8)&0xFF;
    if((ret = uart_enable_divisor_latch(base_addr))) return ret;
    printf("dlm,dll=0x%02X%02X\n", dlm, dll);
    if(sys_outb(base_addr+UART_DLL, dll)) return WRITE_ERROR;
    if(sys_outb(base_addr+UART_DLM, dlm)) return WRITE_ERROR;
    if((ret = util_sys_inb(base_addr+UART_DLM, &dlm))) return ret;
    printf("dlm=0x%02X\n", dlm);
    printf("%s, L91\n", __func__);
    return SUCCESS;
}

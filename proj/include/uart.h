#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#include "uart_macros.h"

typedef struct {
    int     base_addr               ;
    uint8_t lcr                     ;
    uint8_t dll                     ;
    uint8_t dlm                     ;
    uint8_t bits_per_char           ;
    uint8_t stop_bits               ;
    uint8_t parity                  ;
    uint8_t break_control         :1;
    uint8_t dlab                  :1;
    uint16_t divisor_latch          ;
    uint8_t ier                     ;
    uint8_t received_data_int     :1;
    uint8_t transmitter_empty_int :1;
    uint8_t receiver_line_stat_int:1;
    uint8_t modem_stat_int        :1;
} uart_config;

int uart_get_config(int base_addr, uart_config *config);
void uart_parse_config(uart_config *config);
void uart_print_config(uart_config config);

int uart_enable_divisor_latch (int base_addr);
int uart_disable_divisor_latch(int base_addr);

int uart_write_config          (int base_addr, uint8_t     config       );
int uart_set_bits_per_character(int base_addr, uint8_t     bits_per_char);
int uart_set_stop_bits         (int base_addr, uint8_t     stop         );
int uart_set_parity            (int base_addr, uart_parity par          );
int uart_set_bit_rate          (int base_addr, float       bit_rate     );

#endif //UART_H_INCLUDED

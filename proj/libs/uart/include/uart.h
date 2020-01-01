#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

#define COM1_ADDR           0x3F8
#define COM2_ADDR           0x2F8
#define COM1_IRQ            4
#define COM2_IRQ            3
#define COM1_VECTOR         0x0C
#define COM2_VECTOR         0x0B

typedef enum {
    uart_parity_none = 0x0,
    uart_parity_odd  = 0x1,
    uart_parity_even = 0x3,
    uart_parity_par1 = 0x5,
    uart_parity_par0 = 0x7
} uart_parity;

typedef struct {
    int     base_addr               ;
    uint8_t lcr                     ;
    uint8_t dll                     ;
    uint8_t dlm                     ;
    uint8_t bits_per_char           ;
    uint8_t stop_bits               ;
    uart_parity parity              ;
    uint8_t break_control         :1;
    uint8_t dlab                  :1;
    uint16_t divisor_latch          ;
    uint8_t ier                     ;
    uint8_t received_data_int     :1;
    uint8_t transmitter_empty_int :1;
    uint8_t receiver_line_stat_int:1;
    uint8_t modem_stat_int        :1;
} uart_config;

int (subscribe_uart_interrupt)(uint8_t interrupt_bit, int *interrupt_id);

int uart_get_config(int base_addr, uart_config *config);
void uart_print_config(uart_config config);

int uart_set_bits_per_character(int base_addr, uint8_t     bits_per_char);
int uart_set_stop_bits         (int base_addr, uint8_t     stop         );
int uart_set_parity            (int base_addr, uart_parity par          );
int uart_set_bit_rate          (int base_addr, float       bit_rate     );

int uart_enable_int_rx (int base_addr);
int uart_disable_int_rx(int base_addr);
int uart_enable_int_tx (int base_addr);
int uart_disable_int_tx(int base_addr);

/// NCTP - Non-critical transmission protocol
int nctp_init(void);
int nctp_set_processor(void (*proc_func)(const uint8_t*, const size_t));
int nctp_free(void);

int nctp_send(size_t num, uint8_t* ptr[], size_t sz[]);

int nctp_ih_err;
void nctp_ih(void);

#endif //UART_H_INCLUDED

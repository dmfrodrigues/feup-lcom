#ifndef UART_MACROS_H_INCLUDED
#define UART_MACROS_H_INCLUDED

#define UART_BITRATE        115200

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

#endif //UART_MACROS_H_INCLUDED

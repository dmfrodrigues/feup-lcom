#ifndef UART_MACROS_H_INCLUDED
#define UART_MACROS_H_INCLUDED

#define UART_BITRATE        115200

#define COM1_ADDR           0x3F8
#define COM2_ADDR           0x2F8
#define COM1_IRQ            4
#define COM2_IRQ            3
#define COM1_VECTOR         0x0C
#define COM2_VECTOR         0x0B

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

#define UART_GET_BITS_PER_CHAR(n) ((n)&UART_BITS_PER_CHAR + 5)
#define UART_GET_STOP_BITS(n)     ((n)&UART_STOP_BITS? 2 : 1)
#define UART_GET_PARITY(n)        (((n)&UART_PARITY       )>>UART_PARITY_POS       )
#define UART_GET_BREAK_CONTROL(n) (((n)&UART_BREAK_CONTROL)>>UART_BREAK_CONTROL_POS)
#define UART_GET_DLAB(n)          (((n)&UART_DLAB         )>>UART_DLAB_POS         )


typedef enum {
    uart_parity_none = 0x0,
    uart_parity_odd  = 0x1,
    uart_parity_even = 0x3,
    uart_parity_par1 = 0x5,
    uart_parity_par0 = 0x7
} uart_parity;

#endif //UART_MACROS_H_INCLUDED

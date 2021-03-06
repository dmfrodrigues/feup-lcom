#include <lcom/lcf.h>

#include "uart.h"

#include "queue.h"
#include "errors.h"

#define UART_BITRATE                            115200
#define UART_WAIT                               20 //microseconds

#define UART_RBR                                0
#define UART_THR                                0
#define UART_IER                                1
#define UART_IIR                                2
#define UART_FCR                                2
#define UART_LCR                                3
#define UART_MCR                                4
#define UART_LSR                                5
#define UART_MSR                                6
#define UART_SR                                 7

#define UART_DLL                                0
#define UART_DLM                                1

/// LCR
#define UART_BITS_PER_CHAR_POS                  0
#define UART_STOP_BITS_POS                      2
#define UART_PARITY_POS                         3
#define UART_BREAK_CONTROL_POS                  6
#define UART_DLAB_POS                           7

#define UART_BITS_PER_CHAR                      (BIT(0) | BIT(1))
#define UART_STOP_BITS                          (BIT(2))
#define UART_PARITY                             (BIT(3) | BIT(4) | BIT(5))
#define UART_BREAK_CONTROL                      (BIT(6))
#define UART_DLAB                               (BIT(7))

#define UART_GET_BITS_PER_CHAR(n)               (((n)&UART_BITS_PER_CHAR) + 5)
#define UART_GET_STOP_BITS(n)                   (((n)&UART_STOP_BITS)? 2 : 1)
#define UART_GET_PARITY(n)                      (((n)&UART_PARITY       )>>UART_PARITY_POS       )
#define UART_GET_BREAK_CONTROL(n)               (((n)&UART_BREAK_CONTROL)>>UART_BREAK_CONTROL_POS)
#define UART_GET_DLAB(n)                        (((n)&UART_DLAB         )>>UART_DLAB_POS         )
#define UART_GET_DIV_LATCH(m,l)                 ((m)<<8 | (l))
#define UART_GET_DLL(n)                         ((n)&0xFF)
#define UART_GET_DLM(n)                         (((n)>>8)&0xFF)

/// IER
#define UART_INT_EN_RX_POS                      0
#define UART_INT_EN_TX_POS                      1
#define UART_INT_EN_RECEIVER_LINE_STAT_POS      2
#define UART_INT_EN_MODEM_STAT_POS              3

#define UART_INT_EN_RX                          (BIT(0))
#define UART_INT_EN_TX                          (BIT(1))
#define UART_INT_EN_RECEIVER_LINE_STAT          (BIT(2))
#define UART_INT_EN_MODEM_STAT                  (BIT(3))

#define UART_GET_INT_EN_RX(n)                   (((n)&UART_INT_EN_RX                )>>UART_INT_EN_RX_POS                )
#define UART_GET_INT_EN_TX(n)                   (((n)&UART_INT_EN_TX                )>>UART_INT_EN_TX_POS                )
#define UART_GET_INT_EN_RECEIVER_LINE_STAT(n)   (((n)&UART_INT_EN_RECEIVER_LINE_STAT)>>UART_INT_EN_RECEIVER_LINE_STAT_POS)
#define UART_GET_INT_EN_MODEM_STAT(n)           (((n)&UART_INT_EN_MODEM_STAT        )>>UART_INT_EN_MODEM_STAT_POS        )

/// LSR
#define UART_RECEIVER_READY_POS                 0
#define UART_OVERRUN_ERROR_POS                  1
#define UART_PARITY_ERROR_POS                   2
#define UART_FRAMING_ERROR_POS                  3
#define UART_TRANSMITTER_EMPTY_POS              5

#define UART_RECEIVER_READY                     (BIT(0))
#define UART_OVERRUN_ERROR                      (BIT(1))
#define UART_PARITY_ERROR                       (BIT(2))
#define UART_FRAMING_ERROR                      (BIT(3))
#define UART_TRANSMITTER_EMPTY                  (BIT(5))

#define UART_GET_RECEIVER_READY(n)              (((n)&UART_RECEIVER_READY           )>>UART_RECEIVER_READY_POS           )
#define UART_GET_OVERRUN_ERROR                  (((n)&UART_OVERRUN_ERROR            )>>UART_OVERRUN_ERROR_POS            )
#define UART_GET_PARITY_ERROR                   (((n)&UART_PARITY_ERROR             )>>UART_PARITY_ERROR_POS             )
#define UART_GET_FRAMING_ERROR                  (((n)&UART_FRAMING_ERROR            )>>UART_FRAMING_ERROR_POS            )
#define UART_GET_TRANSMITTER_EMPTY(n)           (((n)&UART_TRANSMITTER_EMPTY        )>>UART_TRANSMITTER_EMPTY_POS        )

/// IIR
#define UART_INT_PEND_POS                       1
#define UART_INT_PEND                           (BIT(3)|BIT(2)|BIT(1))

/// FCR
#define UART_EN_FIFOS_POS                       0
#define UART_CLEAR_RCVR_POS                     1
#define UART_CLEAR_XMIT_POS                     2
#define UART_FIFO_TRIGGER_POS                   6

#define UART_EN_FIFOS                           (BIT(0))
#define UART_CLEAR_RCVR                         (BIT(1))
#define UART_CLEAR_XMIT                         (BIT(2))
#define UART_FIFO_TRIGGER                       (BIT(6)|BIT(7))

#define UART_TRIGGER_LEVEL01                    (0<<UART_FIFO_TRIGGER_POS)
#define UART_TRIGGER_LEVEL04                    (1<<UART_FIFO_TRIGGER_POS)
#define UART_TRIGGER_LEVEL08                    (2<<UART_FIFO_TRIGGER_POS)
#define UART_TRIGGER_LEVEL14                    (3<<UART_FIFO_TRIGGER_POS)

#define UART_GET_IF_INT_PEND(n)                 (!((n)&1))
typedef enum {
    uart_int_receiver_line_stat = (         BIT(1) | BIT(0)),
    uart_int_rx                 = (         BIT(1)         ),
    uart_int_char_timeout_fifo  = (BIT(2) | BIT(1)         ),
    uart_int_tx                 = (                  BIT(0)),
    uart_int_modem_stat         = (0)
} uart_int_code;
#define UART_GET_INT_PEND(n)                    ((uart_int_code)(((n)&UART_INT_PEND)>>UART_INT_PEND_POS))

int (subscribe_uart_interrupt)(uint8_t interrupt_bit, int *interrupt_id) {
    if (interrupt_id == NULL) return 1;
    *interrupt_id = interrupt_bit;
    return (sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, interrupt_id));
}

static void uart_parse_config(uart_config *config){
    /// LCR
    config->bits_per_char          = UART_GET_BITS_PER_CHAR     (config->lcr);
    config->stop_bits              = UART_GET_STOP_BITS         (config->lcr);
    config->parity                 = UART_GET_PARITY            (config->lcr); if((config->parity & BIT(0)) == 0) config->parity = uart_parity_none;
    config->break_control          = UART_GET_BREAK_CONTROL     (config->lcr);
    config->dlab                   = UART_GET_DLAB              (config->lcr);
    /// IER
    config->received_data_int      = UART_GET_INT_EN_RX                (config->ier);
    config->transmitter_empty_int  = UART_GET_INT_EN_TX                (config->ier);
    config->receiver_line_stat_int = UART_GET_INT_EN_RECEIVER_LINE_STAT(config->ier);
    config->modem_stat_int         = UART_GET_INT_EN_MODEM_STAT        (config->ier);
    /// DIV LATCH
    config->divisor_latch          = (uint16_t)UART_GET_DIV_LATCH(config->dlm, config->dll);
}

static int uart_get_lcr(int base_addr, uint8_t *p){
    return util_sys_inb(base_addr+UART_LCR, p);
}
static int uart_set_lcr(int base_addr, uint8_t config){
    if(sys_outb(base_addr+UART_LCR, config)) return WRITE_ERROR;
    return SUCCESS;
}
static int uart_get_lsr(int base_addr, uint8_t *p){
    return util_sys_inb(base_addr+UART_LSR, p);
}
static int uart_get_iir(int base_addr, uint8_t *p){
    return util_sys_inb(base_addr+UART_IIR, p);
}

static int uart_enable_divisor_latch(int base_addr){
    int ret = SUCCESS;
    uint8_t conf; if((ret = uart_get_lcr(base_addr, &conf))) return ret;
    return uart_set_lcr(base_addr, conf | UART_DLAB);
}
static int uart_disable_divisor_latch(int base_addr){
    int ret = SUCCESS;
    uint8_t conf; if((ret = uart_get_lcr(base_addr, &conf))) return ret;
    return uart_set_lcr(base_addr, conf & (~UART_DLAB));
}

static int uart_get_ier(int base_addr, uint8_t *p){
    int ret;
    if((ret = uart_disable_divisor_latch(base_addr))) return ret;
    return util_sys_inb(base_addr+UART_IER, p);
}
static int uart_set_ier(int base_addr, uint8_t n){
    int ret;
    if((ret = uart_disable_divisor_latch(base_addr))) return ret;
    if(sys_outb(base_addr+UART_IER, n)) return WRITE_ERROR;
    return SUCCESS;
}

int uart_get_config(int base_addr, uart_config *config){
    int ret = SUCCESS;

    config->base_addr = base_addr;

    if((ret = uart_get_lcr(base_addr, &config->lcr))) return ret;

    if((ret = uart_get_ier(base_addr, &config->ier))) return ret;

    if((ret = uart_enable_divisor_latch (base_addr))) return ret;
    if((ret = util_sys_inb(base_addr+UART_DLL, &config->dll   ))) return ret;
    if((ret = util_sys_inb(base_addr+UART_DLM, &config->dlm   ))) return ret;
    if((ret = uart_disable_divisor_latch(base_addr))) return ret;

    uart_parse_config(config);
    return ret;
}
void uart_print_config(uart_config config){

    printf("%s configuration:\n", (config.base_addr == COM1_ADDR ? "COM1" : "COM2"));
    printf("\tLCR = 0x%X: %d bits per char\t %d stop bits\t", config.lcr, config.bits_per_char, config.stop_bits);
    if((config.parity&BIT(0)) == 0) printf("NO parity\n");
    else switch(config.parity){
        case uart_parity_none: printf("NO parity\n"      ); break;
        case uart_parity_odd : printf("ODD parity\n"     ); break;
        case uart_parity_even: printf("EVEN parity\n"    ); break;
        case uart_parity_par1: printf("parity bit is 1\n"); break;
        case uart_parity_par0: printf("parity bit is 0\n"); break;
        //default              : printf("invalid\n"        ); break;
    }
    printf("\tDLM = 0x%02X DLL=0x%02X: bitrate = %d bps\n", config.dlm, config.dll, UART_BITRATE/config.divisor_latch);
    printf("\tIER = 0x%02X: Rx interrupts: %s\tTx interrupts: %s\n", config.ier,
        (config.received_data_int     ? "ENABLED":"DISABLED"),
        (config.transmitter_empty_int ? "ENABLED":"DISABLED"));
}

int uart_set_bits_per_character(int base_addr, uint8_t bits_per_char){
    if(bits_per_char < 5 || bits_per_char > 8) return INVALID_ARG;
    int ret = SUCCESS;
    bits_per_char = (bits_per_char-5)&0x3;
    uint8_t conf; if((ret = uart_get_lcr(base_addr, &conf))) return ret;
    conf = (conf & (~UART_BITS_PER_CHAR)) | bits_per_char;
    return uart_set_lcr(base_addr, conf);
}
int uart_set_stop_bits(int base_addr, uint8_t stop){
    if(stop != 1 && stop != 2) return INVALID_ARG;
    int ret = SUCCESS;
    stop -= 1;
    stop = (uint8_t)((stop&1)<<2);
    uint8_t conf; if((ret = uart_get_lcr(base_addr, &conf))) return ret;
    conf = (conf & (~UART_STOP_BITS)) | stop;
    return uart_set_lcr(base_addr, conf);
}
int uart_set_parity(int base_addr, uart_parity par){
    int ret = SUCCESS;
    uint8_t parity = (uint8_t)(par << 3);
    uint8_t conf; if((ret = uart_get_lcr(base_addr, &conf))) return ret;
    conf = (conf & (~UART_PARITY)) | parity;
    return uart_set_lcr(base_addr, conf);
}
int uart_set_bit_rate(int base_addr, uint32_t bit_rate){
    int ret = SUCCESS;
    uint16_t latch = (uint16_t)(UART_BITRATE/bit_rate);
    uint8_t dll = UART_GET_DLL(latch);
    uint8_t dlm = UART_GET_DLM(latch);
    if((ret = uart_enable_divisor_latch(base_addr))) return ret;
    if(sys_outb(base_addr+UART_DLL, dll)) return WRITE_ERROR;
    if(sys_outb(base_addr+UART_DLM, dlm)) return WRITE_ERROR;
    if((ret = uart_disable_divisor_latch(base_addr))) return ret;
    return SUCCESS;
}

static int uart_get_char(int base_addr, uint8_t *p){
    int ret;
    if((ret = uart_disable_divisor_latch(base_addr))) return ret;
    return util_sys_inb(base_addr+UART_RBR, p);
}
static int uart_send_char(int base_addr, uint8_t c){
    int ret;
    if((ret = uart_disable_divisor_latch(base_addr))) return ret;
    if(sys_outb(base_addr+UART_THR, c)) return WRITE_ERROR;
    return SUCCESS;
}
static int uart_receiver_ready(int base_addr){
    uint8_t lsr;
    if(uart_get_lsr(base_addr, &lsr)) return false;
    return UART_GET_RECEIVER_READY(lsr);
}
static int uart_transmitter_empty(int base_addr){
    uint8_t lsr;
    if(uart_get_lsr(base_addr, &lsr)) return false;
    return UART_GET_TRANSMITTER_EMPTY(lsr);
}

int uart_enable_int_rx(int base_addr){
    int ret;
    uint8_t ier;
    if((ret = uart_get_ier(base_addr, &ier))) return ret;
    ier |= UART_INT_EN_RX;
    return uart_set_ier(base_addr, ier);
}
int uart_disable_int_rx(int base_addr){
    int ret;
    uint8_t ier;
    if((ret = uart_get_ier(base_addr, &ier))) return ret;
    ier &= ~UART_INT_EN_RX;
    return uart_set_ier(base_addr, ier);
}
int uart_enable_int_tx(int base_addr){
    int ret;
    uint8_t ier;
    if((ret = uart_get_ier(base_addr, &ier))) return ret;
    ier |= UART_INT_EN_TX;
    return uart_set_ier(base_addr, ier);
}
int uart_disable_int_tx(int base_addr){
    int ret;
    uint8_t ier;
    if((ret = uart_get_ier(base_addr, &ier))) return ret;
    ier &= ~UART_INT_EN_TX;
    return uart_set_ier(base_addr, ier);
}

static int uart_set_fcr(int base_addr, uint8_t n){
    if(sys_outb(base_addr+UART_FCR, n)) return WRITE_ERROR;
    return SUCCESS;
}
static int uart_enable_fifos(int base_addr, uint8_t trigger_lvl){
    uint8_t fcr = UART_EN_FIFOS | UART_CLEAR_RCVR | UART_CLEAR_XMIT;
    switch(trigger_lvl){
        case  1: fcr |= UART_TRIGGER_LEVEL01; break;
        case  4: fcr |= UART_TRIGGER_LEVEL04; break;
        case  8: fcr |= UART_TRIGGER_LEVEL08; break;
        case 14: fcr |= UART_TRIGGER_LEVEL14; break;
        default: return INVALID_ARG;
    }
    return uart_set_fcr(base_addr, fcr);
}
static int uart_disable_fifos(int base_addr){
    return uart_set_fcr(base_addr, UART_CLEAR_RCVR | UART_CLEAR_XMIT);
}

/// NCTP

//#define NCTP_START      0x80
//#define NCTP_END        0xFF
#define NCTP_OK         0xFF
#define NCTP_NOK        0x00
#define NCTP_ALIGN      4
#define NCTP_FILLER     0x4E

static queue_t *out = NULL;
static queue_t *in  = NULL;
static void (*process)(const uint8_t*, const size_t) = NULL;

int nctp_init(void){
    out = queue_ctor(); if(out == NULL) return NULL_PTR;
    in  = queue_ctor(); if(in  == NULL) return NULL_PTR;
    //return SUCCESS;
    return uart_enable_fifos(COM1_ADDR, NCTP_ALIGN);
}
int nctp_dump(void){
    int ret;
    if((ret = nctp_free())) return ret;
    return nctp_init();
}
int nctp_set_processor(void (*proc_func)(const uint8_t*, const size_t)){
    process = proc_func;
    return SUCCESS;
}
int nctp_free(void){
    int ret = SUCCESS; int r;
    while(!queue_empty(out)){
        free(queue_top(out));
        queue_pop(out);
    } queue_dtor(out);
    while(!queue_empty(in)){
        free(queue_top(in));
        queue_pop(in);
    } queue_dtor(in);
    if((r = uart_disable_fifos(COM1_ADDR))) ret = r;
    return ret;
}

int nctp_send(size_t num, const uint8_t *const *ptr, const size_t *const sz){
    int ret;
    uint16_t sz_total = 0;{
        for(size_t i = 0; i < num; ++i)
            sz_total += sz[i];
    }
    uint8_t *tmp;
    tmp = malloc(sizeof(uint8_t)); *tmp = *((uint8_t*)(&sz_total)+0); queue_push(out, tmp);
    tmp = malloc(sizeof(uint8_t)); *tmp = *((uint8_t*)(&sz_total)+1); queue_push(out, tmp);
    for(size_t i = 0; i < num; ++i){
        const uint8_t *p = ptr[i]; const size_t s = sz[i];
        for(size_t j = 0; j < s; ++j, ++p){
            tmp = malloc(sizeof(uint8_t)); *tmp = *p; queue_push(out, tmp);
        }
    }
    uint32_t total_message = sz_total+2;
    uint32_t num_fillers = (NCTP_ALIGN - total_message%NCTP_ALIGN)%NCTP_ALIGN;
    for(size_t i = 0; i < num_fillers; ++i){
        tmp = malloc(sizeof(uint8_t)); *tmp = NCTP_FILLER; queue_push(out, tmp);
    }

    if(uart_transmitter_empty(COM1_ADDR)){
        if((ret = uart_send_char(COM1_ADDR, *(uint8_t*)queue_top(out)))) return ret;
        queue_pop(out);
    }
    return SUCCESS;
}
static int nctp_transmit(void){
    while(!queue_empty(out) && uart_transmitter_empty(COM1_ADDR)){
        int ret = uart_send_char(COM1_ADDR, *(uint8_t*)queue_top(out));
        queue_pop(out);
        if(ret) return ret;
        //return ret;
    }/*else*/ return SUCCESS;
}

static void nctp_process_received(){
    uint16_t sz = 0;{
        uint8_t sz0 = *(uint8_t*)queue_top(in); free(queue_top(in)); queue_pop(in);
        uint8_t sz1 = *(uint8_t*)queue_top(in); free(queue_top(in)); queue_pop(in);
        *((uint8_t*)(&sz)+0) = sz0;
        *((uint8_t*)(&sz)+1) = sz1;
    }
    uint8_t *p = malloc(sz*sizeof(uint8_t));
    for(uint16_t i = 0; i < sz; ++i){
        p[i] = *(uint8_t*)queue_top(in);
        free(queue_top(in)); queue_pop(in);
    }
    if(process != NULL) process(p, sz);
    free(p);
}

static int num_bytes_to_receive = 0;
static uint16_t szbytes_to_receive = 0;
static uint8_t size0 = 0;
static uint8_t received_so_far = 0;
static int nctp_receive(void){
    int ret;
    uint8_t c;
    int counter_to_process = 0;

    while(uart_receiver_ready(COM1_ADDR)){
        if((ret = uart_get_char(COM1_ADDR, &c))) return ret;
        uint8_t *tmp = malloc(sizeof(uint8_t)); *tmp = c;

        if       (szbytes_to_receive){ // gotta receive 2nd size byte and update num_bytes
            *((uint8_t*)(&num_bytes_to_receive)+0) = size0;
            *((uint8_t*)(&num_bytes_to_receive)+1) = c;
            szbytes_to_receive = 0;
        } else if(num_bytes_to_receive > 0){
            /* Now I know there are no more size bytes to receive.
             * If there are normal bytes to receive, do this*/
             --num_bytes_to_receive;
             if(num_bytes_to_receive == 0) ++counter_to_process;
        } else {
            /* Now I know I am not expecting anything.
             * The fact I received something means it is a filler, or the 1st size byte.
             * If received_so_far == 0, then it is not a filler, but rather the 1st size byte. */
             if(received_so_far == 0){
                 size0 = c;
                 szbytes_to_receive = 1;
             }else{
                 received_so_far = (received_so_far+1)%NCTP_ALIGN;
                 continue;
             }
        }
        queue_push(in, tmp);
        received_so_far = (received_so_far+1)%NCTP_ALIGN;
    }
    while(counter_to_process-- > 0) nctp_process_received();
    return SUCCESS;
}

static int nctp_ih_err = SUCCESS;
int (nctp_get_ih_error)(void){ return nctp_ih_err; }
void nctp_ih(void){
    uint8_t iir;
    printf("l452\n");
    if((nctp_ih_err = uart_get_iir(COM1_ADDR, &iir))) return;
    printf("l454\n");
    if(UART_GET_IF_INT_PEND(iir)){

        printf("l457\n");
        switch(UART_GET_INT_PEND(iir)){
            case uart_int_rx: nctp_receive (); break;
            case uart_int_tx: nctp_transmit(); break;
            case uart_int_receiver_line_stat: break;
            case uart_int_modem_stat: break;
            case uart_int_char_timeout_fifo: nctp_receive (); break;
            //default: break;
        }

        printf("l467\n");
    }

    printf("l466\n");
}

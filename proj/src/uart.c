#include <lcom/lcf.h>

#include "uart.h"

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
    config->divisor_latch          = UART_GET_DIV_LATCH(config->dlm, config->dll);
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
        case uart_parity_odd : printf("ODD parity\n"     ); break;
        case uart_parity_even: printf("EVEN parity\n"    ); break;
        case uart_parity_par1: printf("parity bit is 1\n"); break;
        case uart_parity_par0: printf("parity bit is 0\n"); break;
        default              : printf("invalid\n"        ); break;
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
    stop = (stop&1)<<2;
    uint8_t conf; if((ret = uart_get_lcr(base_addr, &conf))) return ret;
    conf = (conf & (~UART_STOP_BITS)) | stop;
    return uart_set_lcr(base_addr, conf);
}
int uart_set_parity(int base_addr, uart_parity par){
    int ret = SUCCESS;
    uint8_t parity = par << 3;
    uint8_t conf; if((ret = uart_get_lcr(base_addr, &conf))) return ret;
    conf = (conf & (~UART_PARITY)) | parity;
    return uart_set_lcr(base_addr, conf);
}
int uart_set_bit_rate(int base_addr, float bit_rate){
    int ret = SUCCESS;
    uint16_t latch = UART_BITRATE/bit_rate;
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

static int uart_has_communication_error(int base_addr){
    int ret;
    uint8_t lsr;
    if((ret = uart_get_lsr(base_addr, &lsr))) return 1;
    lsr &= (UART_OVERRUN_ERROR &
            UART_PARITY_ERROR  &
            UART_FRAMING_ERROR);
    return (lsr ? 1 : 0);
}

#include "nctp.h"

#define NCTP_START      0x80
#define NCTP_END        0xFF
#define NCTP_TRIES      3
#define NCTP_OK         0xFF
#define NCTP_NOK        0x00
#define NCTP_MAX_SIZE   1024 //in bytes

int nctp_send_char_poll(int base_addr, uint8_t  c){
    for(int i = 0; i < NCTP_TRIES; ++i)
        if(uart_transmitter_empty(base_addr))
            return uart_send_char(base_addr, c);
    return TIMEOUT_ERROR;
}
int nctp_get_char_poll (int base_addr, uint8_t *p){
    for(int i = 0; i < NCTP_TRIES; ++i)
        if(uart_receiver_ready(base_addr))
            return uart_get_char(base_addr, p);
    return TIMEOUT_ERROR;
}

int nctp_expect_ok(int base_addr){
    int ret;
    uint8_t ok;
    if((ret = nctp_get_char_poll(base_addr, &ok))) return ret;
    int cnt = 0;
    while(ok){
        cnt += ok&1;
        ok >>= 1;
    }
    if(cnt > 4) return SUCCESS;
    else        return NOK;
}

int nctp_send_char_try(int base_addr, uint8_t  c){
    int ret;
    for(size_t k = 0; k < NCTP_TRIES; ++k){
        if((ret = nctp_send_char_poll(base_addr, c))) return ret;
        if(nctp_expect_ok(base_addr) == SUCCESS) return SUCCESS;
    }
    return TRANS_FAILED;
}
int nctp_get_char_try (int base_addr, uint8_t *p){
    int ret;
    for(size_t k = 0; k < NCTP_TRIES; ++k){
        if((ret = nctp_get_char_poll (base_addr, p))) return ret;
        if(!uart_has_communication_error(base_addr))
            return nctp_send_char_try(base_addr, NCTP_OK ); //If it does not have any errors
    }
    if((ret = nctp_send_char_poll(base_addr, NCTP_NOK))) return ret;
    return TRANS_FAILED;
}

int nctp_send(int port, size_t num, uint8_t* ptr[], size_t sz[]){
    {
        int cnt = 0;
        for(size_t i = 0; i < num; ++i){
            cnt += sz[i];
            if(cnt > NCTP_MAX_SIZE) return TRANS_REFUSED;
        }
    }

    int base_addr;{
        switch(port){
            case 1: base_addr = COM1_ADDR; break;
            case 2: base_addr = COM2_ADDR; break;
            default: return INVALID_ARG;
        }
    }

    int ret;

    if((ret = uart_disable_int_rx(base_addr))) return ret;
    if((ret = uart_disable_int_tx(base_addr))) return ret;

    if((ret = nctp_send_char_try(base_addr, NCTP_START))) return ret;
    for(size_t i = 0; i < num; ++i){
        uint8_t *p = ptr[i]; size_t s = sz[i];
        for(size_t j = 0; j < s; ++j, ++p)
            if((ret = nctp_send_char_try(base_addr, *p)))
                return ret;
    }
    if((ret = nctp_send_char_try(base_addr, NCTP_END))) return ret;

    return SUCCESS;
}
int ntcp_get(int port, uint8_t *dest){
    int base_addr;{
        switch(port){
            case 1: base_addr = COM1_ADDR; break;
            case 2: base_addr = COM2_ADDR; break;
            default: return INVALID_ARG;
        }
    }

    int ret;
    free(dest);
    dest = malloc(NCTP_MAX_SIZE*sizeof(uint8_t)); size_t i = 0;
    if(dest == NULL) return NULL_PTR;
    uint8_t c;
    if((ret = nctp_get_char_try (base_addr, &c     ))) return ret;
    while(true){
        if(i >= NCTP_MAX_SIZE) return TRANS_REFUSED;
        if((ret = nctp_get_char_try (base_addr, &c))) return ret;
        if(c == NCTP_END) break;
        else              dest[i] = c;
        ++i;
    }
    return SUCCESS;
}

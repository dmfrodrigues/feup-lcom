#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

/**
 * @defgroup uart uart
 * @brief UART module.
 *
 * @{
 */

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

/**
 * @brief Subscribes UART interrupts and disables Minix Default IH.
 * @param interrupt_bit Bit of interrupt vector that will be set when UART interrupt is pending
 * @param interrupt_id UART interrupt ID to specify the UART interrupt in other calls
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int (subscribe_uart_interrupt)(uint8_t interrupt_bit, int *interrupt_id);
/**
 * @brief Get UART configuration.
 * @param   base_addr   Base address of the serial port whose configuration we want
 * @param   config      Pointer to uart_config where the read configuration will be stored
 * @return SUCCESS if operation is successful, other value otherwise
 */
int uart_get_config(int base_addr, uart_config *config);
/**
 * @brief Print to stdout a UART configuration in a human-friendly way.
 * @param   config  UART configuration to be printed
 */
void uart_print_config(uart_config config);

/**
 * @brief Set number of bits per character.
 *
 * Valid numbers of bits per char go from 5 to 8.
 * @param   base_addr       Base address of serial port
 * @param   bits_per_char   Number of bits per char
 * @return                  SUCCESS if operation is successful, other value otherwise
 */
int uart_set_bits_per_character(int base_addr, uint8_t     bits_per_char);
/**
 * @brief Set number of stop bits.
 *
 * Valid numbers of stop bits are 1 and 2.
 * @param   base_addr   Base address of serial port
 * @param   stop        Number of stop bits
 * @return              SUCCESS if operation is successful, other value otherwise
 */
int uart_set_stop_bits         (int base_addr, uint8_t     stop         );
/**
 * @brief Set parity scheme to be used.
 * @param   base_addr   Base address of serial port
 * @param   par         Parity scheme
 * @return              SUCCESS if operation is successful, other value otherwise
 */
int uart_set_parity            (int base_addr, uart_parity par          );
/**
 * @brief Set bit rate.
 *
 * Valid numbers of bit rates go up to 115200 bps.
 * @param   base_addr       Base address of serial port
 * @param   bit_rate        Number of bits per second
 * @return                  SUCCESS if operation is successful, other value otherwise
 */
int uart_set_bit_rate          (int base_addr, uint32_t    bit_rate     );

/**
 * @brief Enable Receiver Register Ready interrupts
 * @param   base_addr   Base address of serial port
 * @return              SUCCESS if operation is successful, other value otherwise
 */
int uart_enable_int_rx (int base_addr);
/**
 * @brief Disable Receiver Register Ready interrupts
 * @param   base_addr   Base address of serial port
 * @return              SUCCESS if operation is successful, other value otherwise
 */
int uart_disable_int_rx(int base_addr);
/**
 * @brief Enable Transmitter Register Empty interrupts
 * @param   base_addr   Base address of serial port
 * @return              SUCCESS if operation is successful, other value otherwise
 */
int uart_enable_int_tx (int base_addr);
/**
 * @brief Disable Transmitter Register Empty interrupts
 * @param   base_addr   Base address of serial port
 * @return              SUCCESS if operation is successful, other value otherwise
 */
int uart_disable_int_tx(int base_addr);

/**
 * @}
 */

/**
* @defgroup nctp    nctp
* @ingroup uart
* @brief NCTP (Non-Critical Transfer Protocol) module.
* @{
*/

/**
 * @brief Initialize NCTP structures.
 * @return SUCCESS if operation is successful, other value otherwise
 */
int nctp_init(void);
/**
 * @brief Dump all the content of the NCTP in/out queues.
 * @return SUCCESS if operation is successful, other value otherwise
 */
int nctp_dump(void);
/**
 * @brief Set function to be called when a full message is received.
 * @return SUCCESS if operation is successful, other value otherwise
 */
int nctp_set_processor(void (*proc_func)(const uint8_t*, const size_t));
/**
 * @brief Free resources taken on NCTP initialization.
 * @return SUCCESS if operation is successful, other value otherwise
 */
int nctp_free(void);

/**
 * @brief Send a message throught serial port COM1
 * @param   num     Number of different blocks to send in the message
 * @param   ptr     Array of length num, with pointers to blocks that should be sent
 * @param   sz      Size of each block that should be sent, in bytes
 * @return SUCCESS if operation is successful, other value otherwise
 */
int nctp_send(size_t num, const uint8_t *const *ptr, const size_t *const sz);
/**
 * @brief Get NCTP interrupt handler error code.
 * @return  SUCCESS if previous interrupt handler call was successful, other value otherwise
 */
int (nctp_get_ih_error)(void);
/**
 * @brief NCTP interrupt handler.
 *
 * @note Must be called on all interrupt cycles. Even if not necessary, it must be
 * called so the NCTP is allowed to dump any messages it might receive, even
 * if those messages are unexpected.
 */
void nctp_ih(void);

/**
 * @}
 */

#endif //UART_H_INCLUDED

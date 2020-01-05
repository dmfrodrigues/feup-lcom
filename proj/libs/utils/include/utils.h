#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

/**
 * @defgroup utils utils
 * @brief Utilities module.
 *
 * @{
 */

/** @brief Get first digit (leftmost digit) of 8-bit BCD */
#define BCD_FIRST(n)     (n >> 4)
/** @brief Get second digit (rightmost digit) of 8-bit BCD */
#define BCD_SECOND(n)    (n & 0x0F)

/**
 * @brief Gets the least significant byte of a 16-bit variable
 * @param val 16-bit variable
 * @param lsb Pointer to a 8-bit variable to store the value of the LSB
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Gets the most significant byte of a 16-bit variable
 * @param val 16-bit variable
 * @param msb Pointer to a 8-bit variable to store the value of the MSB
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief sys_inb wrapper
 * @param port Port to read from
 * @param value Pointer to byte to store value read
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 */
int (util_sys_inb)(int port, uint8_t *value);

/**
 * @brief Unsubcribes Interrupts
 * @param interrupt_id Interrupt ID, value via arguments on subscription of the interrupt_id
 * @see subscribe_kbc_interrupt, subscribe_timer_interrupt
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 */
int (unsubscribe_interrupt)(int *interrupt_id);

/**
 * @brief Gets the minimum value out of two values.
 * @param a     First value
 * @param b     Second value
 * @return  The minimum of the two values
 */
int16_t min16(int16_t a, int16_t b);
uint16_t umin16(uint16_t a, uint16_t b);
int32_t min32(int32_t a, int32_t b);
double  dmin (double  a, double  b);

/**
 * @brief Gets the maximum value out of two values.
 * @param a     First value
 * @param b     Second value
 * @return  The maximum of the two values
 */
int16_t max16(int16_t a, int16_t b);
uint16_t umax16(uint16_t a, uint16_t b);
int32_t max32(int32_t a, int32_t b);
double  dmax (double  a, double  b);

/**
 * @brief Get the absolute value of a double.
 * @param   a   Argument
 * @return      Absolute value of a
 */
double dabs(double a);

/**
 * @brief Compare if two doubles are approximately equal (within 1e-10).
 * @param   a   First value
 * @param   b   Second value
 * @return      true if the two numbers are approximately equal, false otherwise
 */
int deq(double a, double b);

/**
 * @}
 */

#endif //UTILS_H_INCLUDED

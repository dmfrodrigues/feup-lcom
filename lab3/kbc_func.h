#ifndef _KBC_FUNC_H_
#define _KBC_FUNC_H_

#include <stdint.h>

#define GET         0 /* @brief Argument to get counter without incrementing */
#define INCREMENT   1 /* @brief Argument for incrementing counter */

/**
 * @brief Subscribes KBC Interrupts and disables Minix Default Keyboard IH
 * @param interrupt_bit Bit of Interrupt Vector that will be set when KBC Interrupt is pending
 * @param interrupt_id KBC Interrupt ID to specify the KBC Interrupt in other calls
 * @return Whether operation was sucessful or not
 */
int (subscribe_kbc_interrupt)(uint8_t interrupt_bit, int *interrupt_id);

/**
 * @brief Unsubcribes KBC Interrupts
 * @param interrupt_id KBC Interrupt ID, value via arguments on subscription of the interrupt_id
 * @see subscribe_kbc_interrupt
 * @return Whether operation was sucessful or not
 */
int (unsubscribe_kbc_interrupt)(int *interrupt_id);

/**
 * @brief KBC Interrupt Handler
 */
void (kbc_ih)(void);

/**
 * @brief Counter for sys_inb calls
 * @param increment Whether the value should be incremented or not
 * @return The current value of the counter
 */
uint32_t sys_inb_counter(int get_increment);

#endif

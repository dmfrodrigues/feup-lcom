#ifndef INTERRUPTS_FUNC_H_INCLUDED
#define INTERRUPTS_FUNC_H_INCLUDED

/**
 * @defgroup interrupts_func interrupts_func
 * @ingroup proj
 * @brief Interrupts functions.
 *
 * @{
 */

#include "libs.h"

/**
 * @brief Subscribes all drivers used (timer->keyboard->mouse)
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int (subscribe_all)(void);

/**
 * @brief Unsubscribes all subscribed interrupts
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int (unsubscribe_all)(void);

/**
 * @brief Calls interrupt handler corresponding to the bit specified by the argument.
 * @param handler Interrupt handler to call
 */
void interrupt_handler(uint8_t handler);

/**
 * @brief Blocking function that waits for an interrupt to occur.
 *
 * Rewrites the provided argument with the interrupt vector it gets.
 *
 * This function decisively reduces the complexity of an interrupt cycle, allowing
 * for each menu to have its own, simplified interrupt cycle.
 * @param   p   Pointer where interrupt vector will be saved
 */
int get_interrupts_vector(uint64_t *p);

/**
 * @}
 */

#endif /* end of include guard: INTERRUPTS_FUNC_H_INCLUDED */

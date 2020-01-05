#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

/**
 * @defgroup    timer   timer
 * @brief   Timer module.
 *
 * @{
 */

/** @brief Timer 0 IRQ line */
#define TIMER0_IRQ     0

/**
 * @brief Subscribes Timer Interrupts and disables Minix Default IH.
 * @param interrupt_bit Bit of Interrupt Vector that will be set when Timer Interrupt is pending
 * @param interrupt_id Timer Interrupt ID to specify the Timer Interrupt in other calls
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int (subscribe_timer_interrupt)(uint8_t interrupt_bit, int *interrupt_id);

/**
 * @brief Timer interrupt handler. Increases interrupt counter.
 */
void (timer_int_handler)(void);

/**
 * @brief Get number of timer interrupts.
 * @return  Number of timer interrupts.
 */
uint32_t (timer_get_no_interrupts)(void);

/**
 * @}
 */

#endif //TIMER_H_INCLUDED

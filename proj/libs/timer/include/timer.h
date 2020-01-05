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

int (subscribe_timer_interrupt)(uint8_t interrupt_bit, int *interrupt_id);

void (timer_int_handler)(void);

uint32_t (timer_get_no_interrupts)(void);

/**
 * @}
 */

#endif //TIMER_H_INCLUDED

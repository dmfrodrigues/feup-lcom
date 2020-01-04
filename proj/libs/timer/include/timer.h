/**
 * This file concerns everything related to the timer
 */

#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#define TIMER0_IRQ     0                                                                /**< @brief Timer 0 IRQ line */

int (subscribe_timer_interrupt)(uint8_t interrupt_bit, int *interrupt_id);

void (timer_int_handler)(void);

uint32_t (timer_get_no_interrupts)(void);

#endif //TIMER_H_INCLUDED

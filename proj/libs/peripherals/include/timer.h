/**
 * This file concerns everything related to the timer
 */

#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

int (subscribe_timer_interrupt)(uint8_t interrupt_bit, int *interrupt_id);

uint32_t no_interrupts;

#endif //TIMER_H_INCLUDED

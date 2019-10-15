#ifndef TIMER_FUNC_H_INCLUDED
#define TIMER_FUNC_H_INCLUDED

#include <stdint.h>

int (subscribe_timer_interrupt)(uint8_t interrupt_bit, int *interrupt_id);

#endif //TIMER_FUNC_H_INCLUDED

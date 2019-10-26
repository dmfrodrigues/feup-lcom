/**
 * This file concerns everything related to the keyboard
 */

#ifndef KBD_H_INCLUDED
#define KBD_H_INCLUDED

int (subscribe_kbc_interrupt)(uint8_t interrupt_bit, int *interrupt_id);

uint8_t scancode[2];
int two_byte_scancode = 0;
int got_error = 0;

void (kbc_ih)(void);

int (kbd_poll)(uint8_t bytes[], uint8_t *size);

#endif //KBD_H_INCLUDED

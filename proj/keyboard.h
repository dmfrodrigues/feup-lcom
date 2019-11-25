/**
 * This file concerns everything related to the keyboard
 */

#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

/**
 * @brief Subscribes Keyboard Interrupts and disables Minix Default IH
 * @param interrupt_bit Bit of Interrupt Vector that will be set when Keyboard Interrupt is pending
 * @param interrupt_id Keyboard Interrupt ID to specify the Keyboard Interrupt in other calls
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int (subscribe_kbc_interrupt)(uint8_t interrupt_bit, int *interrupt_id);

uint8_t scancode[2];
int done;
int sz;
int got_error_keyboard;

void (kbc_ih)(void);

int (keyboard_poll)(uint8_t bytes[], uint8_t *size);

#endif //KEYBOARD_H_INCLUDED

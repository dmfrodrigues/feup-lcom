/**
 * This file concerns everything related to the keyboard
 */

#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#include "kbc.h"

#define W_MAKE_CODE             0x11    /** @brief W Make Code */
#define W_BREAK_CODE            0x91    /** @brief W Break Code */
#define A_MAKE_CODE             0x1E    /** @brief A Make Code */
#define A_BREAK_CODE            0x9E    /** @brief A Break Code */
#define S_MAKE_CODE             0x1F    /** @brief S Make Code */
#define S_BREAK_CODE            0x9F    /** @brief S Break Code */
#define D_MAKE_CODE             0x20    /** @brief D Make Code */
#define D_BREAK_CODE            0xA0    /** @brief D Break Code */
#define CTRL_MAKE_CODE          0x1D    /** @brief CTRL Make Code */
#define CTRL_BREAK_CODE         0x9D    /** @brief CTRL Break Code */
#define PLUS_MAKE_CODE          0x1A    /** @brief Plus (+) Make Code */
#define PLUS_BREAK_CODE         0x9A    /** @brief Plus (+) Break Code */
#define MINUS_MAKE_CODE         0x35    /** @brief Minus (-) Make Code */
#define MINUS_BREAK_CODE        0xB5    /** @brief Minus (-) Break Code */
#define ENTER_MAKE_CODE         0x1C
#define ENTER_BREAK_CODE        0x9C
#define BACKSPACE_MAKE_CODE     0x0E
#define BACKSPACE_BREAK_CODE    0x8E

#define ESC_MAKE_CODE           0x01    /** @brief ESC Make Code */
#define ESC_BREAK_CODE          0x81    /** @brief ESC Break Code */

/**
 * @brief Subscribes Keyboard Interrupts and disables Minix Default IH
 * @param interrupt_bit Bit of Interrupt Vector that will be set when Keyboard Interrupt is pending
 * @param interrupt_id Keyboard Interrupt ID to specify the Keyboard Interrupt in other calls
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int (subscribe_kbc_interrupt)(uint8_t interrupt_bit, int *interrupt_id);


const uint8_t* keyboard_get_scancode(void);
int keyboard_get_done(void);
int keyboard_get_size(void);
int keyboard_get_error(void);

void (kbc_ih)(void);

int (keyboard_poll)(uint8_t bytes[], uint8_t *size);

#endif //KEYBOARD_H_INCLUDED

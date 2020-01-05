#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

/**
 * @defgroup    keyboard    keyboard
 * @ingroup kbc
 * @brief Keyboard module.
 *
 * @{
 */

#include "kbc.h"

/** @brief W Make Code */
#define W_MAKE_CODE             0x11
/** @brief W Break Code */
#define W_BREAK_CODE            0x91
/** @brief A Make Code */
#define A_MAKE_CODE             0x1E
/** @brief A Break Code */
#define A_BREAK_CODE            0x9E
/** @brief S Make Code */
#define S_MAKE_CODE             0x1F
/** @brief S Break Code */
#define S_BREAK_CODE            0x9F
/** @brief D Make Code */
#define D_MAKE_CODE             0x20
/** @brief D Break Code */
#define D_BREAK_CODE            0xA0
/** @brief CTRL Make Code */
#define CTRL_MAKE_CODE          0x1D
/** @brief CTRL Break Code */
#define CTRL_BREAK_CODE         0x9D
/** @brief Plus (+) Make Code */
#define PLUS_MAKE_CODE          0x1A
/** @brief Plus (+) Break Code */
#define PLUS_BREAK_CODE         0x9A
/** @brief Minus (-) Make Code */
#define MINUS_MAKE_CODE         0x35
/** @brief Minus (-) Break Code */
#define MINUS_BREAK_CODE        0xB5
/** @brief Enter Make Code */
#define ENTER_MAKE_CODE         0x1C
/** @brief Enter Break Code */
#define ENTER_BREAK_CODE        0x9C
/** @brief Backspace Make Code */
#define BACKSPACE_MAKE_CODE     0x0E
/** @brief Backspace Break Code */
#define BACKSPACE_BREAK_CODE    0x8E
/** @brief ESC Make Code */
#define ESC_MAKE_CODE           0x01
/** @brief ESC Break Code */
#define ESC_BREAK_CODE          0x81

/**
 * @brief Subscribes Keyboard Interrupts and disables Minix Default IH.
 * @param interrupt_bit Bit of Interrupt Vector that will be set when Keyboard Interrupt is pending
 * @param interrupt_id Keyboard Interrupt ID to specify the Keyboard Interrupt in other calls
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int (subscribe_kbc_interrupt)(uint8_t interrupt_bit, int *interrupt_id);

/**
 * @brief Get scancode read from keyboard.
 * @return  Pointer to begin of scancode array (array with size 2)
 */
const uint8_t* keyboard_get_scancode(void);
/**
 * @brief Get state of reading (done).
 * @return  1 if done, 0 otherwise
 */
int keyboard_get_done(void);
/**
 * @brief Get size of read scancode (1 or 2).
 * @return Size of scancode
 */
int keyboard_get_size(void);
/**
 * @brief Get error code of previous run of kbc_ih, if an error occured.
 * @return  Error code, or 0 if successful
 */
int keyboard_get_error(void);
/**
 * @brief KBC interrupt handler. Reads scancodes.
 */
void (kbc_ih)(void);
/**
 * @brief Poll keyboard for scancodes.
 * @param   bytes   Array to put read bytes (up to 2)
 * @param   size    Number of ready bytes (1 or 2)
 * @return  SUCCESS if operation was successful, other value otherwise
 */
int (keyboard_poll)(uint8_t bytes[], uint8_t *size);

/**
 * @}
 */

#endif //KEYBOARD_H_INCLUDED

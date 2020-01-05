#ifndef KBC_H_INCLUDED
#define KBC_H_INCLUDED

/**
 * @defgroup    kbc kbc
 * @brief   KBC module.
 *
 * @{
 */

/* KBC IRQ Line */
/** @brief KBC Controller IRQ Line */
#define KBC_IRQ     1
/** @brief Mouse IRQ Line */
#define MOUSE_IRQ   12

/* Delay for KBC */
/** @brief KBC Response Delay */
#define DELAY           20000
/** @brief Number of tries to issue command before timeout */
#define KBC_NUM_TRIES   20

/* I/O Ports Addresses */
/** @brief Address to send commands to KBC */
#define KBC_CMD     0x64
/** @brief Address to write KBC Command Arguments */
#define KBC_CMD_ARG 0x60
/** @brief KBC Status Register address */
#define STATUS_REG  0x64
/** @brief Address of Output Buffer of KBC */
#define OUTPUT_BUF  0x60

/* KBC Commands */
/** @brief Read KBC Command Byte */
#define READ_KBC_CMD    0x20
/** @brief Write KBC Command Byte */
#define WRITE_KBC_CMD   0x60
/** @brief KBC Diagnostic Tests */
#define KBC_SELF_TEST   0xAA
/** @brief Tests Keyboard Clock and Data lines */
#define KBC_INT_TEST    0xAB
/** @brief Disable KBC Interface */
#define KBC_INT_DISABLE 0xAD
/** @brief Enable KBC Interface */
#define KBC_INT_ENABLE  0xAE
/** @brief Disable Mouse */
#define MOUSE_DISABLE   0xA7
/** @brief Enable Mouse */
#define MOUSE_ENABLE    0xA8
/** @brief Tests Mouse data line */
#define MOUSE_INT_TEST  0xA9
/** @brief Write a byte directly to the mouse */
#define MOUSE_WRITE_B   0xD4

/* Status Byte Masking */
/** @brief Output Buffer State */
#define OUT_BUF_FUL     BIT(0)
/** @brief Input Buffer State */
#define IN_BUF_FULL     BIT(1)
/** @brief System Flag */
#define SYS_FLAG        BIT(2)
/** @brief Identifier of type of byte in input buffer */
#define DATA_CMD_WRITE  BIT(3)
/** @brief Keyboard inihibited */
#define INH_FLAG        BIT(4)
/** @brief Mouse Data */
#define AUX_MOUSE       BIT(5)
/** @brief Time Out Error - Invalid Data */
#define TIME_OUT_REC    BIT(6)
/** @brief Parity Error - Invalid Data */
#define PARITY_ERROR    BIT(7)

/* Scancode Constants */
/** @brief ESC Break Code */
#define ESC_BREAK_CODE  0x81
/** @brief First byte of a two byte Scancode */
#define TWO_BYTE_CODE   0xE0
/** @brief Bit to distinguish between Make code and Break code */
#define BREAK_CODE_BIT  BIT(7)

/* Command byte masks */
/** @brief Enable Keyboard Interrupts */
#define INT_KBD         BIT(0)
/** @brief Enable Mouse Interrupts */
#define INT_MOU         BIT(1)
/** @brief Disable Keyboard */
#define DIS_KBD         BIT(4)
/** @brief Disable Mouse */
#define DIS_MOU         BIT(5)

/**
 * @brief High-level function that reads the command byte of the KBC
 * @param cmd Pointer to variable where command byte read from KBC will be stored
 * @return 0 if operation was successful, 1 otherwise
 */
int (kbc_read_cmd)(uint8_t *cmd);

/**
 * @brief High-level function that changes the command byte of the KBC
 * @param cmd New value for command byte of KBC
 * @return 0 if operation was successful, 1 otherwise
 */
int (kbc_change_cmd)(uint8_t cmd);

/**
 * @brief High-level function that restores KBC to normal state
 *
 * High-level function that restores KBC to normal state, because lcf_start
 * changes the command byte of KBC. If this function is not used, there is a
 * chance that the keyboard and keyboard interrupts remain disabled.
 * @return 0 if operation was successful, 1 otherwise
 */
int (kbc_restore_keyboard)();

/**
 * @brief Low-level function to issue a command to keyboard
 * @param cmd command to be issued
 * @return 0 if operation was successful, 1 otherwise
 */
int (kbc_issue_cmd)(uint8_t cmd);

/**
 * @brief Low-level function to issue an argument of a command
 * @param arg argument to be issued
 * @return 0 if operation was successful, 1 otherwise
 */
int (kbc_issue_arg)(uint8_t arg);

/**
 * @brief Low-level function for reading byte from keyboard
 *
 * Low-level function for reading byte from keyboard. Waits until output buffer
 * is full
 * @param byte Pointer to variable where byte read from keyboard will be stored
 * @return 0 if operation was successful, 1 otherwise
 */
int (kbc_read_byte)(uint8_t *byte);

/**
 * @}
 */

#endif //KBC_H_INCLUDED

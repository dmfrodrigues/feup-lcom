#ifndef MOUSE_H_INCLUDED
#define MOUSE_H_INCLUDED

#include <stdint.h>

/**
 * @brief Subscribes Mouse Interrupts and disables Minix Default IH
 * @param interrupt_bit Bit of Interrupt Vector that will be set when Mouse Interrupt is pending
 * @param interrupt_id Mouse Interrupt ID to specify the Mouse Interrupt in other calls
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int (subscribe_mouse_interrupt)(uint8_t interrupt_bit, int *interrupt_id);

struct packet (mouse_parse_packet)(const uint8_t *packet_bytes);

int (mouse_set_data_report)(int on);

/**
 * @brief Reads data byte from mouse
 * <summary>
 * Polls the mouse till data is available for reading
 * </summary>
 * @param data Pointer to variable where byte read from mouse will be stored
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int (mouse_read_data)(uint8_t *data);

/**
 * @brief Issues command to mouse
 * <summary>
 * Issues command to mouse, returns error after two consecutive errors reported by the acknowledgment byte
 * </summary>
 * @param cmd Command to be issued
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int (mouse_issue_cmd)(uint32_t cmd);

/**
 * @brief Reads byte from mouse
 * <summary>
 * Reads byte from mouse, giving error if exceeds number of tries to read
 * </summary>
 * @param byte Pointer to variable where byte read from mouse will be stored
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int (mouse_read_byte)(uint8_t *byte);

int (mouse_read_ack)(uint8_t *byte);

/**
 * @brief Converts 9-bit number to 16-bit with sign extension
 * @param sign_bit  Sign bit identifiying the signal of the number
 * @param byte      Least significant byte that will be extended
 * @return Extended 9-bit number
 */
int16_t (sign_extend_byte)(uint8_t sign_bit, uint8_t byte);

#endif //MOUSE_H_INCLUDED

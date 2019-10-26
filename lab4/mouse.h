#ifndef _MOUSE_FUNC_H_
#define _MOUSE_FUNC_H_

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

#endif /* end of include guard: _MOUSE_FUNC_H_ */

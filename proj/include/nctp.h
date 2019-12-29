#ifndef NCTP_H_INCLUDED
#define NCTP_H_INCLUDED

#include "uart.h"

int nctp_get_char_poll   (int base_addr, uint8_t *p);
int nctp_send_char_poll  (int base_addr, uint8_t  c);

/**
 * @brief Send stuff to COM #port.
 * @param   port    Port to send message to
 * @param   num     Number of pairs of address/numbytes to send
 */
int nctp_send(int base_addr, size_t num, uint8_t* ptr[], size_t sz[]);

int nctp_get (int base_addr, uint8_t **dest);

#endif //NCTP_H_INCLUDED

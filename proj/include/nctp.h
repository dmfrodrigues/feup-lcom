#ifndef NCTP_H_INCLUDED
#define NCTP_H_INCLUDED

#include "uart.h"

int nctp_init(void);
int nctp_free(void);

int nctp_send(size_t num, uint8_t* ptr[], size_t sz[]);

int nctp_ih_err;
void nctp_ih(void);

#endif //NCTP_H_INCLUDED

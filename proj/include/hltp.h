#ifndef HLTP_H_INCLUDED
#define HLTP_H_INCLUDED

#include "nctp.h"

int hltp_send_string(int base_addr, const char *p);

int hltp_get_string(int base_addr, char **p);

#endif //HLTP_H_INCLUDED

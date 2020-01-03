#ifndef HLTP_H_INCLUDED
#define HLTP_H_INCLUDED

#include "uart.h"
#include "proj_structures.h"

typedef enum{
    hltp_type_invalid = 0x00,
    hltp_type_string  = 0x53,
    hltp_type_host    = 0x54,
    hltp_type_remote  = 0x55
} hltp_type;

int hltp_send_string(const char *p);
int hltp_send_host_info(const host_info_t *p);
int hltp_send_remote_info(const remote_info_t *p);
hltp_type hltp_interpret(const uint8_t *p, const size_t sz, void **dest);
#endif //HLTP_H_INCLUDED

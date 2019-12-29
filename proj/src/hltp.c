#include <lcom/lcf.h>

#include "hltp.h"

#include "nctp.h"

int hltp_send_string(int port, const char *p){
    uint8_t* ptr[] = {p};
    size_t    sz[] = {strlen(p)};
    return nctp_send(port, 1, ptr, sz);
}

int hltp_get_string(int port, char *p){
    return nctp_get(port, p);
}

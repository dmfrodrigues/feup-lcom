#include <lcom/lcf.h>

#include "hltp.h"

#include "nctp.h"

int hltp_send_string(int base_addr, const char *p){
    uint8_t* ptr[1]; ptr[0] = (uint8_t*)p;
    size_t    sz[1]; sz[0] = strlen(p)+1;
    return nctp_send(base_addr, 1, ptr, sz);
}

int hltp_get_string(int base_addr, char **p){
    return nctp_get(base_addr, (uint8_t**)p);
}

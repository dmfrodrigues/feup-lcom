#include <lcom/lcf.h>

#include "hltp.h"

#include "uart.h"

static void* hltp_interpret_string(const uint8_t *p, const size_t sz){
    void *ret = malloc((sz+1)*sizeof(char));
    for(size_t i = 0; i < sz; ++i) ((char*)ret)[i] = p[i];
    ((char*)ret)[sz] = '\0';
    return ret;
}
int hltp_send_string(const char *p){
    uint8_t type = hltp_type_string;
    uint8_t* ptr[2]; ptr[0] = &type; ptr[1] = (uint8_t*)p;
    size_t    sz[2]; sz [0] =     1; sz [1] = strlen(p);
    return nctp_send(2, ptr, sz);
}

hltp_type hltp_interpret(const uint8_t *p, const size_t sz, void **dest){
    uint8_t ret = p[0];
    switch(ret){
        case hltp_type_string: *dest = hltp_interpret_string(p+1, sz-1); break;
        default: *dest = NULL; break;
    }
    return ret;
}

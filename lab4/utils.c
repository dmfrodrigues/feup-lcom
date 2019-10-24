#include "errors.h"

#include <lcom/lcf.h>

#include <stdint.h>

int (util_sys_inb)(int port, uint8_t *value) {
    if(value == NULL) return NULL_PTR;
    uint32_t n = 0;
    if(sys_inb(port, &n)) return READ_ERROR;
    *value = n;
    return SUCCESS;
}

#include <lcom/lcf.h>

#include <stdint.h>

int (util_sys_inb)(int port, uint8_t *value) {
    if(value == NULL) return 1;
    uint32_t n = 0;
    if(sys_inb(port, &n)) return 1;
    *value = n;
    return 0;
}

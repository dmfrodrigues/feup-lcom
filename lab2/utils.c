#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
    if (lsb == NULL)
        return 1;

    *lsb = val;
    return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
    if (*msb == NULL)
        return 1;
    *msb = val >> 8;
    return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
    /* To be implemented by the students */
    if(value == NULL) return 1;
    uint32_t n = 0;
    int ret = sys_inb(port, &n);
    *value = n;
    return ret;
}

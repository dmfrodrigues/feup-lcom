#include <lcom/lcf.h>

#include <stdint.h>

/* Byte Masks */
#define LSB 0xFF /**< @brief mask to filter least significant byte */

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
    if (lsb == NULL) return 1;

    *lsb = val & LSB;
    return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
    if (msb == NULL) return 1;
    *msb = val >> 8;
    return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
    if(value == NULL) return 1;
    uint32_t n = 0;
    if(sys_inb(port, &n)) return 1;
    *value = n & LSB;
    return 0;
}

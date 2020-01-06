#include <lcom/lcf.h>

#include "utils.h"

#include <stdint.h>
#include "errors.h"

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
    if (lsb == NULL) return NULL_PTR;
    *lsb = (uint8_t)val;
    return SUCCESS;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
    if (msb == NULL) return NULL_PTR;
    *msb = (val >> 8);
    return SUCCESS;
}

int (util_sys_inb)(int port, uint8_t *value) {
    if(value == NULL) return NULL_PTR;
    uint32_t n = 0;
    if(sys_inb(port, &n)) return READ_ERROR;
    *value = (uint8_t)n;
    return SUCCESS;
}

int (unsubscribe_interrupt)(int *interrupt_id) {
    if (interrupt_id == NULL) return NULL_PTR;
    if(sys_irqrmpolicy(interrupt_id)) return UNSBCR_ERROR;
    return SUCCESS;
}

int16_t min_16(int16_t a, int16_t b){ return (b < a ? b : a); }
uint16_t umin_16(uint16_t a, uint16_t b){ return (b < a ? b : a); }
int32_t min32(int32_t a, int32_t b){ return (b < a ? b : a); }
double  dmin (double  a, double  b){ return (b < a ? b : a); }

int16_t max_16(int16_t a, int16_t b){ return (a < b ? b : a); }
uint16_t max_u16(uint16_t a, uint16_t b){ return (a < b ? b : a); }
int32_t max32(int32_t a, int32_t b){ return (a < b ? b : a); }
double  max_d (double  a, double  b){ return (a < b ? b : a); }

double abs_d(double a)          { return (a < 0 ? -a: a); }

int eq_d(double a, double b){ return (abs_d(a-b) < 1e-10 ? true : false); }

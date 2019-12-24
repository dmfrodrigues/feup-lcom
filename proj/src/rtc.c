#include "rtc.h"
#include "rtc_macros.h"

#include <lcom/lcf.h>

#include "utils.h"
#include "errors.h"

int (subscribe_rtc_interrupt)(uint8_t interrupt_bit, int *interrupt_id) {
    if (interrupt_id == NULL) return NULL_PTR;
    *interrupt_id = interrupt_bit;
    if (sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, interrupt_id)) return SBCR_ERROR;
    return SUCCESS;
}

int (rtc_read_register)(uint32_t reg, uint8_t *data) {
    if (sys_outb(RTC_ADDR_REG, reg)) return WRITE_ERROR;

    if (util_sys_inb(RTC_DATA_REG, data)) return READ_ERROR;

    return SUCCESS;
}

int (rtc_write_register)(uint32_t reg, uint8_t data) {
    if (sys_outb(RTC_ADDR_REG, reg)) return WRITE_ERROR;

    if (sys_outb(RTC_DATA_REG, data)) return WRITE_ERROR;

    return SUCCESS;
}

int (rtc_check_update)(void) {
    uint8_t data;
    int r;

    if ((r = rtc_read_register(RTC_REG_A, &data))) return r;

    return (data & UIP) != 0;
}

int (rtc_set_updates)(int on) {
    uint8_t data;
    int r;
    if (on) {
        if ((r = rtc_read_register(RTC_REG_B, &data))) return r;

        data &= ~SET;

        if ((r = rtc_write_register(RTC_REG_B, data))) return r;

    } else {
        while (rtc_check_update());

        if ((r = rtc_read_register(RTC_REG_B, &data))) return r;

        data |= SET;

        if ((r = rtc_write_register(RTC_REG_B, data))) return r;
    }
    return SUCCESS;
}

int (rtc_read_time)(uint8_t *time) {

    int r;
    //if ((r = rtc_set_updates(false))) return r;
    while (rtc_check_update());

    uint8_t hour, min, sec;

    if ((r = rtc_read_register(RTC_SEC, &sec)))    return r;
    if ((r = rtc_read_register(RTC_MIN, &min)))    return r;
    if ((r = rtc_read_register(RTC_HOUR, &hour)))  return r;

    //if ((r = rtc_set_updates(true))) return r;

    time[0] = BCD_FIRST(sec)*10   + BCD_SECOND(sec);
    time[1] = BCD_FIRST(min)*10   + BCD_SECOND(min);
    time[2] = BCD_FIRST(hour)*10  + BCD_SECOND(hour);

    return SUCCESS;
}

int (rtc_read_date)(uint8_t *date) {
    int r;
    //if ((r = rtc_set_updates(false))) return r;
    while (rtc_check_update());

    uint8_t year, month, day, weekday;

    if ((r = rtc_read_register(RTC_WEEK_DAY,    &weekday)))     return r;
    if ((r = rtc_read_register(RTC_MONTH_DAY,   &day)))         return r;
    if ((r = rtc_read_register(RTC_MONTH,       &month)))       return r;
    if ((r = rtc_read_register(RTC_YEAR,        &year)))        return r;

    //if ((r = rtc_set_updates(true))) return r;

    date[0] = weekday;
    date[1] = BCD_FIRST(day)*10     + BCD_SECOND(day);
    date[2] = BCD_FIRST(month)*10   + BCD_SECOND(month);
    date[3] = BCD_FIRST(year)*10    + BCD_SECOND(year);

    return SUCCESS;
}

#include "rtc.h"
#include "rtc_macros.h"

#include <lcom/lcf.h>

#include "utils.h"
#include "errors.h"

// RTC IRQ Line

#define KBC_IRQ   8  /* @brief KBC IRQ Line */

// RTC Ports

#define RTC_ADDR_REG        0x70
#define RTC_DATA_REG        0x71

// RTC Registers (Byte)

#define RTC_SEC             0
#define RTC_SEC_ALARM       1
#define RTC_MIN             2
#define RTC_MIN_ALARM       3
#define RTC_HOUR            4
#define RTC_HOUR_ALARM      5
#define RTC_WEEK_DAY        6
#define RTC_MONTH_DAY       7
#define RTC_MONTH           8
#define RTC_YEAR            9
#define RTC_REG_A           10
#define RTC_REG_B           11
#define RTC_REG_C           12
#define RTC_REG_D           13

// Register A

#define RS0     BIT(0)  /** @brief Rate selector */
#define RS1     BIT(1)  /** @brief Rate selector */
#define RS2     BIT(2)  /** @brief Rate selector */
#define RS3     BIT(3)  /** @brief Rate selector */
#define UIP     BIT(7)  /** @brief Update in progress */

// Register B

#define DSE             BIT(0)  /** @brief Enable Daylight Saving Time */
#define HOUR_FORM       BIT(1)  /** @brief Set to 1 for 24h format (0-23), 0 to 12h format (1-12) */
#define DM              BIT(2)  /** @brief Set to 1 if registers are in binary, 0 if in BCD */
#define SQWE            BIT(3)  /** @brief Enable square wave generation */
#define UIE             BIT(4)  /** @brief Enable Update interrupts */
#define AIE             BIT(5)  /** @brief Enable alarm interrupts */
#define PIE             BIT(6)  /** @brief Enable periodic interrupts */
#define SET             BIT(7)  /** @brief Set to 1 to inhibit updates of time/date registers */

// Register C

#define UF      BIT(4)  /** @brief Update Interrupt Pending */
#define AF      BIT(5)  /** @brief Alarm Interrupt Pending */
#define PF      BIT(6)  /** @brief Periodic Interrupt Pending */
#define IRQF    BIT(7)  /** @brief IRQ Line Active */

// Register D

#define VRT     BIT(7)  /** @brief Valid RAM/time - If set to 0 RTC reading aren't valid */

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

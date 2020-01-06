#include <lcom/lcf.h>

#include "rtc.h"

#include "utils.h"
#include "errors.h"

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

static uint8_t s = 0, m = 0, h = 0;

int (subscribe_rtc_interrupt)(uint8_t interrupt_bit, int *interrupt_id) {
    if (interrupt_id == NULL) return NULL_PTR;
    *interrupt_id = interrupt_bit;
    if (sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, interrupt_id)) return SBCR_ERROR;
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

int (rtc_set_updates_int)(int on) {
    uint8_t data;
    int r;
    if (on) {
        if ((r = rtc_read_register(RTC_REG_B, &data))) return r;

        data |= UIE;

        if ((r = rtc_write_register(RTC_REG_B, data))) return r;

    } else {
        if ((r = rtc_read_register(RTC_REG_B, &data))) return r;

        data &= ~UIE;

        if ((r = rtc_write_register(RTC_REG_B, data))) return r;
    }
    return SUCCESS;
}

int (rtc_read_sec)(uint8_t *sec) {
    return rtc_read_register(RTC_SEC, sec);
}

int (rtc_read_min)(uint8_t *min) {
    return rtc_read_register(RTC_MIN, min);
}

int (rtc_read_hour)(uint8_t *hour) {
    return rtc_read_register(RTC_HOUR, hour);
}

int (rtc_read_weekday)(uint8_t *weekday) {
    return rtc_read_register(RTC_WEEK_DAY, weekday);
}

int (rtc_read_day)(uint8_t *day) {
    return rtc_read_register(RTC_MONTH_DAY, day);
}

int (rtc_read_month)(uint8_t *month) {
    return rtc_read_register(RTC_MONTH, month);
}

int (rtc_read_year)(uint8_t *year) {
    return rtc_read_register(RTC_YEAR, year);
}

int (rtc_read_time)(uint8_t *time) {

    time[0] = BCD_FIRST(s)*10   + BCD_SECOND(s);
    time[1] = BCD_FIRST(m)*10   + BCD_SECOND(m);
    time[2] = BCD_FIRST(h)*10  + BCD_SECOND(h);

    return SUCCESS;
}

int (rtc_read_date)(uint8_t *date) {
    int r;

    uint8_t year=0, month=0, day=0, weekday=0;
    uint8_t year2=0, month2=0, day2=0, weekday2=0;

    do {
        year2 = year;
        month2 = month;
        day2 = day;
        weekday2 = weekday;

        if ((r = rtc_read_weekday(&weekday))) return r;
        if ((r = rtc_read_day(&day))) return r;
        if ((r = rtc_read_month(&month))) return r;
        if ((r = rtc_read_year(&year))) return r;

    } while (year != year2 || month != month2 || day != day2 || weekday != weekday2);

    date[0] = weekday;
    date[1] = BCD_FIRST(day)*10     + BCD_SECOND(day);
    date[2] = BCD_FIRST(month)*10   + BCD_SECOND(month);
    date[3] = BCD_FIRST(year)*10    + BCD_SECOND(year);

    return SUCCESS;
}

void rtc_ih() {
    uint8_t data;

    if (rtc_read_register(RTC_REG_C, &data)) return;

    if (data & UF) {
        if (rtc_read_sec(&s)) return;
        if (rtc_read_min(&m)) return;
        if (rtc_read_hour(&h)) return;
    }
}

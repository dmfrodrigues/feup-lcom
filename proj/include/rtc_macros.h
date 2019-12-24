#ifndef RTC_MACROS_H_INCLUDED
#define RTC_MACROS_H_INCLUDED

#include <lcom/lcf.h>

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

#endif /* end of include guard: RTC_MACROS_H_INCLUDED */

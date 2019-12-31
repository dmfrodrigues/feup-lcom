#include <lcom/lcf.h>

#include "timer.h"

#define TIMER_FREQ     1193182                                                          /**< @brief clock frequency for timer in PC and AT */
#define TIMER_MIN_FREQ (TIMER_FREQ/UINT16_MAX) + ((TIMER_FREQ % UINT16_MAX) ? 1 : 0)    /**< @brief mininum frequency for timer */

/* I/O port addresses */

#define TIMER_0    0x40         /**< @brief Timer 0 count register */
#define TIMER_1    0x41         /**< @brief Timer 1 count register */
#define TIMER_2    0x42         /**< @brief Timer 2 count register */
#define TIMER_CTRL 0x43         /**< @brief Control register */

#define SPEAKER_CTRL 0x61       /**< @brief Register for speaker control  */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0   0x00              /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1   BIT(6)            /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2   BIT(7)            /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Read Back Command */

/* Register selection: bits 5 and 4 */

#define TIMER_LSB     BIT(4)                    /**< @brief Initialize Counter LSB only */
#define TIMER_MSB     BIT(5)                    /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB)   /**< @brief Initialize LSB first and MSB afterwards */
#define TIMER_INMODE_MASK 0x30                  /**< @brief Mask for Timer Counter Mode */
#define TIMER_INMODE_POS  4                     /**< @brief Bit position of Timer Counter Mode */

/* Operating mode: bits 3, 2 and 1 */

#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN BIT(2)            /**< @brief Mode 2: rate generator */
#define TIMER_MODE_MASK 0x0E             /**< @brief Mask for mode */
#define TIMER_MODE_POS  1                /**< @brief Position of smallest bit from mode */
#define TIMER_MODE_2ALT 0x6              /**< @brief Alternative notation for mode 2 */
#define TIMER_MODE_3ALT 0x7              /**< @brief Alternative notation for mode 3 */
#define TIMER_MODE_RED2 0x03             /**< @brief Reduce 3-bit mode to 2-bit mode */

/* Counting mode: bit 0 */

#define TIMER_BCD 0x01 /**< @brief Count in BCD */
#define TIMER_BIN 0x00 /**< @brief Count in binary */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_  BIT(5)         /**< @brief Read counter value on read-back (0 to activate) */
#define TIMER_RB_STATUS_ BIT(4)         /**< @brief Read status value on read-back (0 to activate) */
#define TIMER_RB_SEL(n)  BIT((n) + 1)   /**< @brief Select timer to read information from */

int (subscribe_timer_interrupt)(uint8_t interrupt_bit, int *interrupt_id) {
    if (interrupt_id == NULL) return 1;
    *interrupt_id = interrupt_bit;
    return (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, interrupt_id));
}

uint32_t no_interrupts = 0;
void (timer_int_handler)() {
    no_interrupts++;
}

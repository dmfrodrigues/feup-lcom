#ifndef RTC_H_INCLUDED
#define RTC_H_INCLUDED

/**
 * @defgroup    rtc rtc
 * @ingroup libs
 * @brief RTC (Real-Time Clock) module.
 *
 * @{
 */

#include <stdint.h>

/**
 * @brief Subscribes RTC Interrupts
 * @param interrupt_bit Bit of Interrupt Vector that will be set when RTC Interrupt is pending
 * @param interrupt_id RTC Interrupt ID to specify the RTC Interrupt in other calls
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int (subscribe_rtc_interrupt)(uint8_t interrupt_bit, int *interrupt_id);

/**
 * @brief Read RTC register.
 * @param   reg     Register to read from
 * @param   data    Pointer to save read data
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int (rtc_read_register)(uint32_t reg, uint8_t *data);
/**
 * @brief Write RTC register.
 * @param   reg     Register to write to
 * @param   data    Data to write in register
 * @return ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 * @see {_ERRORS_H_::errors}
 */
int (rtc_write_register)(uint32_t reg, uint8_t data);

/**
 * @brief Checks if there's an update in progress.
 * @return  The value of the flag UIP, or ERROR_CODE if error occurs.
 */
int (rtc_check_update)(void);

/**
 * @brief Enables/Disables updates of time/date registers.
 * @param   on  zero to disable, any other value to enable
 * @return  ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 */
int (rtc_set_updates)(int on);

/**
 * @brief Reads time from RTC.
 * @param   time  Pointer to array of 3 bytes to store the information about time (hours, minutes, seconds)
 * @return  ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 */
int (rtc_read_time)(uint8_t *time);

/**
 * @brief Reads date from RTC.
 * @param   date  Pointer to array of 4 bytes to store the information about date (year, month, day, weekday)
 * @return  ERROR_CODE code representing the result of the operation, SUCCESS code is returned if everything is OK
 */
int (rtc_read_date)(uint8_t *date);

/**
 * @}
 */

#endif /* end of include guard: RTC_H_INCLUDED */

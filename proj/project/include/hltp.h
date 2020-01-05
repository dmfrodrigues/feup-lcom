#ifndef HLTP_H_INCLUDED
#define HLTP_H_INCLUDED

/**
 * @defgroup    hltp    hltp
 * @ingroup proj
 * @brief HLTP (High-Level Transfer Protocol) module.
 *
 * @{
 */

#include "uart.h"
#include "proj_structures.h"

/**
 * @brief Data types that HLTP is familiar with (i.e., can send and intepret).
 */
typedef enum{
    hltp_type_invalid = 0x00,
    hltp_type_string  = 0x53,
    hltp_type_host    = 0x48,
    hltp_type_remote  = 0x52,
    hltp_type_bullet  = 0x42
} hltp_type;

/**
 * @brief Send string.
 * @param   p   Pointer to string to be sent
 * @return  SUCCESS if operation was successful, other value otherwise
 */
int hltp_send_string(const char *p);
/**
 * @brief Send host_info.
 * @param   p   Pointer to host_info to be sent
 * @return  SUCCESS if operation was successful, other value otherwise
 */
int hltp_send_host_info(const host_info_t *p);
/**
 * @brief Send remote_info.
 * @param   p   Pointer to remote_info to be sent
 * @return  SUCCESS if operation was successful, other value otherwise
 */
int hltp_send_remote_info(const remote_info_t *p);
/**
 * @brief Send bullet_info.
 * @param   p   Pointer to bullet_info to be sent
 * @return  SUCCESS if operation was successful, other value otherwise
 */
int hltp_send_bullet_info(const bullet_info_t *p);
/**
 * @brief Interpret message received as one of the structs HLTP knows.
 * @param   p       Pointer to beginning of message
 * @param   sz      Size of message
 * @param   dest    Pointer to void*, to where the interpreted message will be saved
 * @return          Code of the interpreted message
 */
hltp_type hltp_interpret(const uint8_t *p, const size_t sz, void **dest);

/**
 * @}
 */

#endif //HLTP_H_INCLUDED

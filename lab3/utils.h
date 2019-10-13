#ifndef _UTILS_H_
#define _UTILS_H_

/**
 * @brief Reads byte from port
 * @param port Adress Port to read from
 * @param value Address to be stored the value read
 * @return Whether operation was sucessful or not
 */
int (util_sys_inb)(int port, uint8_t *value);

#endif

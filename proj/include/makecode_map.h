#ifndef MAKE_CODE_MAP_H_
#define MAKE_CODE_MAP_H_

/**
 * @defgroup makecode_map makecode_map
 * @brief Makecode map.
 *
 * @{
 */

#include <stdint.h>

#define BASE_CODE    0x02
#define ERROR_CODE      0

/**
 * @brief Maps make code into char
 * @param code      Make code to map.
 * @return 0 if the char isn't valid, otherwise the character.
 */
char (map_makecode)(uint8_t code);

/**
 * @}
 */

#endif /* end of include guard: MAKE_CODE_MAP_H_ */

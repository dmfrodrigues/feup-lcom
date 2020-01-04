#include "makecode_map.h"

static const char makecode_map[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\'', ERROR_CODE /*«*/, ERROR_CODE /*escape*/,
                                    ERROR_CODE /*tab*/, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '+', ERROR_CODE /* ´ */, ERROR_CODE /*enter*/,
                                    ERROR_CODE /*ctrl*/, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ERROR_CODE /* Ç */, ERROR_CODE /* º */, '\\', ERROR_CODE /*lshift*/, '~',
                                    'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '-', ERROR_CODE /*rshift*/, ERROR_CODE /*unkown*/, ERROR_CODE /*alt*/, ' ' /*space*/};

static const int size = sizeof(makecode_map);

char (map_makecode)(uint8_t code) {
    int key = code - BASE_CODE;
    if (key < 0 || key >= size) return ERROR_CODE;
    return makecode_map[key];
}

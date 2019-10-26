#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#ifdef LAB3
    uint32_t sys_inb_counter;
#endif

int (util_sys_inb)(int port, uint8_t *value);

#endif //UTILS_H_INCLUDED

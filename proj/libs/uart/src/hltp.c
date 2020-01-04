#include <lcom/lcf.h>

#include "hltp.h"

#include "uart.h"

static void* hltp_interpret_string(const uint8_t *p, const size_t sz){
    void *ret = malloc((sz+1)*sizeof(char));
    for(size_t i = 0; i < sz; ++i) ((char*)ret)[i] = p[i];
    ((char*)ret)[sz] = '\0';
    return ret;
}
int hltp_send_string(const char *p){
    uint8_t type = hltp_type_string;
    uint8_t* ptr[2]; ptr[0] = &type; ptr[1] = (uint8_t*)p;
    size_t    sz[2]; sz [0] =     1; sz [1] = strlen(p);
    return nctp_send(2, ptr, sz);
}

static host_info_t* hltp_interpret_host_info(const uint8_t *p) {
    host_info_t *ret = (host_info_t*)malloc(sizeof(host_info_t));
    size_t pos = 0;
    // players information
    memcpy(ret, p + pos, sizeof(double)*8);
    pos += sizeof(double)*8;
    // size of arrays
    memcpy(&(ret->no_bullets), p + pos, sizeof(size_t));
    pos += sizeof(size_t);
    size_t sz = ret->no_bullets;
    // array containing the X positions of the bullets
    (ret->bullets_x) = (double*)malloc(sizeof(double)*sz);
    memcpy((ret->bullets_x), p + pos, sizeof(double)*sz);
    pos += sizeof(double)*sz;
    // array containing the Y positions of the bullets
    (ret->bullets_y) = (double*)malloc(sizeof(double)*(ret->no_bullets));
    memcpy((ret->bullets_y), p + pos, sizeof(double)*sz);
    pos += sizeof(double)*sz;
    // array containing the X velocity of the bullets
    (ret->bullets_vx) = (double*)malloc(sizeof(double)*(ret->no_bullets));
    memcpy((ret->bullets_vx), p + pos, sizeof(double)*sz);
    pos += sizeof(double)*sz;
    // array containing the Y velocity of the bullets
    (ret->bullets_vy) = (double*)malloc(sizeof(double)*(ret->no_bullets));
    memcpy((ret->bullets_vy), p + pos, sizeof(double)*sz);
    pos += sizeof(double)*sz;
    // array containing the shooter id of the bullets
    (ret->bullets_shooter) = (bool*)malloc(sizeof(bool)*(ret->no_bullets));
    memcpy((ret->bullets_shooter), p + pos, sizeof(bool)*sz);

    return ret;
}
int hltp_send_host_info(const host_info_t *p) {

    uint8_t type = hltp_type_host;
    uint8_t* ptr[15]; size_t sz[15];
    ptr[0]  = (uint8_t*)&   type                  ;     sz[0] = 1;
    ptr[1]  = (uint8_t*)&p->host_x                ;     sz[0] = sizeof(double);
    ptr[2]  = (uint8_t*)&p->host_y                ;     sz[0] = sizeof(double);
    ptr[3]  = (uint8_t*)&p->host_health           ;     sz[0] = sizeof(double);
    ptr[4]  = (uint8_t*)&p->host_current_health   ;     sz[0] = sizeof(double);
    ptr[5]  = (uint8_t*)&p->remote_x              ;     sz[0] = sizeof(double);
    ptr[6]  = (uint8_t*)&p->remote_y              ;     sz[0] = sizeof(double);
    ptr[7]  = (uint8_t*)&p->remote_health         ;     sz[0] = sizeof(double);
    ptr[8]  = (uint8_t*)&p->remote_current_health ;     sz[0] = sizeof(double);
    ptr[9]  = (uint8_t*)&p->no_bullets            ;     sz[0] = sizeof(size_t);
    ptr[10] = (uint8_t*) p->bullets_x             ;     sz[0] = sizeof(double) * p->no_bullets;
    ptr[11] = (uint8_t*) p->bullets_y             ;     sz[0] = sizeof(double) * p->no_bullets;
    ptr[12] = (uint8_t*) p->bullets_vx            ;     sz[0] = sizeof(double) * p->no_bullets;
    ptr[13] = (uint8_t*) p->bullets_vy            ;     sz[0] = sizeof(double) * p->no_bullets;
    ptr[14] = (uint8_t*) p->bullets_shooter       ;     sz[0] = sizeof(double) * p->no_bullets;
    return nctp_send(2, ptr, sz);
}

static remote_info_t* hltp_interpret_remote_info(const uint8_t *p) {
    remote_info_t *ret = (remote_info_t*)malloc(sizeof(remote_info_t));
    size_t pos = 0;
    // keys pressed
    memcpy(&(ret->remote_keys_pressed), p + pos, sizeof(keys_t));
    pos += sizeof(keys_t);
    // mouse positions
    memcpy(&(ret->remote_mouse_x), p + pos, sizeof(int32_t));
    pos += sizeof(int32_t);
    memcpy(&(ret->remote_mouse_y), p + pos, sizeof(int32_t));
    pos += sizeof(int32_t);
    // bullets shot
    memcpy(&(ret->bullets_shot), p + pos, sizeof(size_t));
    pos += sizeof(size_t);
    size_t sz = ret->bullets_shot;
    // array containing the X positions of the bullets
    (ret->bullets_x) = (double*)malloc(sizeof(double)*sz);
    memcpy((ret->bullets_x), p + pos, sizeof(double)*sz);
    pos += sizeof(double)*sz;
    // array containing the Y positions of the bullets
    (ret->bullets_y) = (double*)malloc(sizeof(double)*sz);
    memcpy((ret->bullets_y), p + pos, sizeof(double)*sz);
    pos += sizeof(double)*sz;
    // array containing the angle of the bullets
    (ret->bullets_angle) = (double*)malloc(sizeof(double)*sz);
    memcpy((ret->bullets_angle), p + pos, sizeof(double)*sz);
    pos += sizeof(double)*sz;

    return ret;
}
int hltp_send_remote_info(const remote_info_t *p) {

    uint8_t type = hltp_type_remote;
    uint8_t* ptr[8]; size_t sz[8];
    ptr[0]  = (uint8_t*)&   type                  ;     sz[0] = 1;
    ptr[1]  = (uint8_t*)&p->remote_keys_pressed   ;     sz[0] = sizeof(keys_t);
    ptr[2]  = (uint8_t*)&p->remote_mouse_x        ;     sz[0] = sizeof(int32_t);
    ptr[3]  = (uint8_t*)&p->remote_mouse_y        ;     sz[0] = sizeof(int32_t);
    ptr[4]  = (uint8_t*)&p->bullets_shot          ;     sz[0] = sizeof(size_t);
    ptr[5]  = (uint8_t*) p->bullets_x             ;     sz[0] = sizeof(double) * p->bullets_shot;
    ptr[6]  = (uint8_t*) p->bullets_y             ;     sz[0] = sizeof(double) * p->bullets_shot;
    ptr[7]  = (uint8_t*) p->bullets_angle         ;     sz[0] = sizeof(double) * p->bullets_shot;
    return nctp_send(2, ptr, sz);
}

hltp_type hltp_interpret(const uint8_t *p, const size_t sz, void **dest){
    uint8_t ret = p[0];
    switch(ret){
        case hltp_type_string: *dest = hltp_interpret_string(p+1, sz-1); break;
        case hltp_type_host  : *dest = hltp_interpret_host_info(p+1); break;
        case hltp_type_remote: *dest = hltp_interpret_remote_info(p+1); break;
        default: *dest = NULL; break;
    }
    return ret;
}

#include <lcom/lcf.h>

#include "hltp.h"

#include "uart.h"

static void* hltp_interpret_string(const uint8_t *p, const size_t sz){
    void *ret = malloc((sz+1)*sizeof(char));
    for(size_t i = 0; i < sz; ++i) ((char*)ret)[i] = (char)p[i];
    ((char*)ret)[sz] = '\0';
    return ret;
}
int hltp_send_string(const char *p){
    uint8_t type = hltp_type_string;
    const uint8_t* ptr[2]; ptr[0] = &type; ptr[1] = (const uint8_t*)p;
    size_t         sz[2]; sz [0] =     1; sz [1] = strlen(p);
    return nctp_send(2, ptr, sz);
}

static host_info_t* hltp_interpret_host_info(const uint8_t *p) {
    host_info_t *ret = (host_info_t*)malloc(sizeof(host_info_t));
    size_t pos = 0;
    // players information
    memcpy(ret, p + pos, sizeof(float)*10);
    pos += sizeof(float)*10;
    // size of arrays
    memcpy(&(ret->no_bullets), p + pos, sizeof(uint8_t));
    pos += sizeof(uint8_t);
    size_t sz = ret->no_bullets;
    // array containing the X positions of the bullets
    (ret->bullets_x) = (float*)malloc(sizeof(float)*sz);
    memcpy((ret->bullets_x), p + pos, sizeof(float)*sz);
    pos += sizeof(float)*sz;
    // array containing the Y positions of the bullets
    (ret->bullets_y) = (float*)malloc(sizeof(float)*(ret->no_bullets));
    memcpy((ret->bullets_y), p + pos, sizeof(float)*sz);
    pos += sizeof(float)*sz;
    // array containing the X velocity of the bullets
    (ret->bullets_vx) = (float*)malloc(sizeof(float)*(ret->no_bullets));
    memcpy((ret->bullets_vx), p + pos, sizeof(float)*sz);
    pos += sizeof(float)*sz;
    // array containing the Y velocity of the bullets
    (ret->bullets_vy) = (float*)malloc(sizeof(float)*(ret->no_bullets));
    memcpy((ret->bullets_vy), p + pos, sizeof(float)*sz);
    pos += sizeof(float)*sz;
    // array containing the shooter id of the bullets
    (ret->bullets_shooter) = (bool*)malloc(sizeof(bool)*(ret->no_bullets));
    memcpy((ret->bullets_shooter), p + pos, sizeof(bool)*sz);

    return ret;
}
int hltp_send_host_info(const host_info_t *p) {

    uint8_t type = hltp_type_host;
    const uint8_t* ptr[17]; size_t sz[17];
    ptr[0]  = (uint8_t*)&   type                  ;     sz[0]   = 1;
    ptr[1]  = (uint8_t*)&p->host_x                ;     sz[1]   = sizeof(float);
    ptr[2]  = (uint8_t*)&p->host_y                ;     sz[2]   = sizeof(float);
    ptr[3]  = (uint8_t*)&p->host_angle            ;     sz[3]   = sizeof(float);
    ptr[4]  = (uint8_t*)&p->host_health           ;     sz[4]   = sizeof(float);
    ptr[5]  = (uint8_t*)&p->host_current_health   ;     sz[5]   = sizeof(float);
    ptr[6]  = (uint8_t*)&p->remote_x              ;     sz[6]   = sizeof(float);
    ptr[7]  = (uint8_t*)&p->remote_y              ;     sz[7]   = sizeof(float);
    ptr[8]  = (uint8_t*)&p->remote_angle          ;     sz[8]   = sizeof(float);
    ptr[9]  = (uint8_t*)&p->remote_health         ;     sz[9]   = sizeof(float);
    ptr[10] = (uint8_t*)&p->remote_current_health ;     sz[10]  = sizeof(float);
    ptr[11] = (uint8_t*)&p->no_bullets            ;     sz[11]  = sizeof(uint8_t);
    ptr[12] = (uint8_t*) p->bullets_x             ;     sz[12]  = sizeof(float) * p->no_bullets;
    ptr[13] = (uint8_t*) p->bullets_y             ;     sz[13]  = sizeof(float) * p->no_bullets;
    ptr[14] = (uint8_t*) p->bullets_vx            ;     sz[14]  = sizeof(float) * p->no_bullets;
    ptr[15] = (uint8_t*) p->bullets_vy            ;     sz[15]  = sizeof(float) * p->no_bullets;
    ptr[16] = (uint8_t*) p->bullets_shooter       ;     sz[16]  = sizeof(float) * p->no_bullets;
    return nctp_send(17, ptr, sz);
}

static remote_info_t* hltp_interpret_remote_info(const uint8_t *p) {
    remote_info_t *ret = (remote_info_t*)malloc(sizeof(remote_info_t));
    size_t pos = 0;
    // keys pressed
    memcpy(&(ret->remote_keys_pressed), p + pos, sizeof(keys_t));
    pos += sizeof(keys_t);
    // mouse positions
    memcpy(&(ret->remote_angle), p + pos, sizeof(double));

    return ret;
}
int hltp_send_remote_info(const remote_info_t *p) {

    uint8_t type = hltp_type_remote;
    const uint8_t* ptr[3]; size_t sz[3];
    ptr[0]  = (uint8_t*)&   type                  ;     sz[0] = 1;
    ptr[1]  = (uint8_t*)&p->remote_keys_pressed   ;     sz[1] = sizeof(keys_t);
    ptr[2]  = (uint8_t*)&p->remote_angle          ;     sz[2] = sizeof(double);
    return nctp_send(3, ptr, sz);
}

static bullet_info_t* hltp_interpret_bullet_info(const uint8_t *p) {
    bullet_info_t *ret = (bullet_info_t*)malloc(sizeof(bullet_info_t));
    memcpy(&(ret->new_bullet), p, sizeof(bool));

    return ret;
}

int hltp_send_bullet_info(const bullet_info_t *p) {
    uint8_t type = hltp_type_bullet;
    const uint8_t* ptr[2]; size_t sz[2];
    ptr[0]  = (uint8_t*)&   type            ;     sz[0] = 1;
    ptr[1]  = (uint8_t*)&p->new_bullet      ;     sz[1] = sizeof(bool);
    return nctp_send(2, ptr, sz);
}

hltp_type hltp_interpret(const uint8_t *p, const size_t sz, void **dest){
    uint8_t ret = p[0];
    switch(ret){
        case hltp_type_string: *dest = hltp_interpret_string     (p+1, sz-1);   break;
        case hltp_type_host  : *dest = hltp_interpret_host_info  (p+1);         break;
        case hltp_type_remote: *dest = hltp_interpret_remote_info(p+1);         break;
        case hltp_type_bullet: *dest = hltp_interpret_bullet_info(p+1);         break;
        default: *dest = NULL; break;
    }
    return ret;
}

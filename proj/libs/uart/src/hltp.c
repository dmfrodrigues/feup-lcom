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
    // struct size
    size_t struct_size = sizeof(double) * (8 + 4*p->no_bullets) + p->no_bullets * sizeof(bool) + sizeof(size_t);
    uint8_t *base = (uint8_t*)malloc(struct_size);
    uint8_t *aux = base;
    // players information
    memcpy(aux, p, sizeof(double) * 8);
    aux = aux + sizeof(double) * 8;
    // size of arrays
    memcpy(aux, &(p->no_bullets), sizeof(size_t));
    aux = aux + sizeof(size_t);
    // array containing the X positions of the bullets
    memcpy(aux, p->bullets_x, sizeof(double) * (p->no_bullets));
    aux = aux + sizeof(double) * (p->no_bullets);
    // array containing the Y positions of the bullets
    memcpy(aux, p->bullets_y, sizeof(double) * (p->no_bullets));
    aux = aux + sizeof(double) * (p->no_bullets);
    // array containing the X velocity of the bullets
    memcpy(aux, p->bullets_vx, sizeof(double) * (p->no_bullets));
    aux = aux + sizeof(double) * (p->no_bullets);
    // array containing the Y velocity of the bullets
    memcpy(aux, p->bullets_vy, sizeof(double) * (p->no_bullets));
    aux = aux + sizeof(double) * (p->no_bullets);
    // array containing the shooter id of the bullets
    memcpy(aux, p->bullets_shooter, sizeof(bool) * (p->no_bullets));

    uint8_t type = hltp_type_host;
    uint8_t* ptr[2]; ptr[0] = &type; ptr[1] = base;
    size_t    sz[2]; sz [0] =     1; sz [1] = struct_size;
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
    // struct size
    size_t struct_size = sizeof(keys_t) + sizeof(int32_t) * 2 + sizeof(size_t) + sizeof(double) * (3 *p->bullets_shot);
    uint8_t *base = (uint8_t*)malloc(struct_size);
    uint8_t *aux = base;
    memcpy(aux, p, sizeof(keys_t));
    aux = aux + sizeof(keys_t);
    memcpy(aux, p, sizeof(int32_t)*2);
    aux = aux + sizeof(int32_t)*2;
    memcpy(aux, &(p->bullets_shot), sizeof(size_t));
    aux = aux + sizeof(size_t);
    memcpy(aux, p->bullets_x, sizeof(double)*(p->bullets_shot));
    aux = aux + sizeof(double)*(p->bullets_shot);
    memcpy(aux, p->bullets_y, sizeof(double)*(p->bullets_shot));
    aux = aux + sizeof(double)*(p->bullets_shot);
    memcpy(aux, p->bullets_angle, sizeof(double)*(p->bullets_shot));

    uint8_t type = hltp_type_remote;
    uint8_t* ptr[2]; ptr[0] = &type; ptr[1] = base;
    size_t    sz[2]; sz [0] =     1; sz [1] = struct_size;
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

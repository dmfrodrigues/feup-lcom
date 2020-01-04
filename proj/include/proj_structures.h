#ifndef PROJ_STRUCTURES_H_INCLUDED
#define PROJ_STRUCTURES_H_INCLUDED

#include <stdint.h>
#include "ent.h"

typedef struct keys {
    uint8_t w_pressed       : 1;
    uint8_t a_pressed       : 1;
    uint8_t s_pressed       : 1;
    uint8_t d_pressed       : 1;
    uint8_t ctrl_pressed    : 1;
    uint8_t plus_pressed    : 1;
    uint8_t minus_pressed   : 1;
    uint8_t lb_pressed      : 1;
} keys_t;

typedef struct __attribute__((packed)) {
    // host
    double  host_x;
    double  host_y;
    double  host_angle;
    double  host_health;
    double  host_current_health;

    // remote
    double  remote_x;
    double  remote_y;
    double  remote_angle;
    double  remote_health;
    double  remote_current_health;

    // bullets
    size_t  no_bullets;
    double *bullets_x;
    double *bullets_y;
    double *bullets_vx;
    double *bullets_vy;
    bool   *bullets_shooter; // false for host, true for remote
} host_info_t;

typedef struct __attribute__((packed)) {
    keys_t  remote_keys_pressed;
    double  remote_angle;
} remote_info_t;

typedef struct __attribute__((packed)) {
    bool   new_bullet;
} bullet_info_t;

host_info_t* host_info_ctor(gunner_t *host, gunner_t *remote);

void host_info_dtor(host_info_t *p);

remote_info_t* remote_info_ctor();

void remote_info_dtor(remote_info_t *p);

bullet_info_t* bullet_info_ctor();

void bullet_info_dtor(bullet_info_t *p);

#endif /* end of include guard: PROJ_STRUCTURES_H_INCLUDED */

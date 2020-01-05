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

typedef struct {
    // host
    int16_t    host_x;
    int16_t    host_y;
    int16_t    host_angle;
    int16_t    host_health;
    int16_t    host_current_health;

    // remote
    int16_t    remote_x;
    int16_t    remote_y;
    int16_t    remote_angle;
    int16_t    remote_health;
    int16_t    remote_current_health;

    // bullets
    uint8_t    no_bullets;
    int16_t   *bullets_x;
    int16_t   *bullets_y;
    int16_t   *bullets_vx;
    int16_t   *bullets_vy;
    bool      *bullets_shooter; // false for host, true for remote
} host_info_t;

typedef struct {
    keys_t  remote_keys_pressed;
    double  remote_angle;
} remote_info_t;

typedef struct {
    bool   new_bullet;
} bullet_info_t;

host_info_t* host_info_ctor(gunner_t *host, gunner_t *remote);

void host_info_dtor(host_info_t *p);

remote_info_t* remote_info_ctor(void);

void remote_info_dtor(remote_info_t *p);

bullet_info_t* bullet_info_ctor(void);

void bullet_info_dtor(bullet_info_t *p);

#endif /* end of include guard: PROJ_STRUCTURES_H_INCLUDED */

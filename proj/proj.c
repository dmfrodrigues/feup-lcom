#include <lcom/lcf.h>
#include <lcom/proj.h>
#include <lcom/liblm.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8254.h"
#include "kbc_macros.h"
#include "graph_macros.h"
#include "mouse_macros.h"
#include "proj_macros.h"
#include "errors.h"

#include "sprite.h"
#include "kbc.h"
#include "graph.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "utils.h"
#include "interrupts_func.h"
#include "proj_func.h"

#include "fast_math.h"
#include <math.h>

#ifdef DIOGO
    #include "shooter.h"
    #include "pistol.xpm"
#endif
#ifdef TELMO
    #include "crosshair.h"
#endif

int main(int argc, char* argv[]) {

    lcf_set_language("EN-US");

    //lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

    //lcf_log_output("/home/lcom/labs/proj/output.txt");

    if (lcf_start(argc, argv)) return 1;

    lcf_cleanup();

    return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {

    int r;

    /// subscribe interrupts
    if (subscribe_all()) { return 1; }

    /// initialize graphics
    if(graph_init(GRAPH_MODE)){
        printf("%s: failed to initalize graphics.\n", __func__);
        if (cleanup()) printf("%s: failed to cleanup.\n", __func__);
        return 1;
    }

    #ifdef DIOGO
        printf("%d\n", 1000000-(int)(1000000*fm_sin(0.5*M_PI)));
        printf("%d\n", (int)(1000000*fm_cos(0.5*M_PI)));

        clock_t t = clock();
        sprite_t *shooter1 = get_shooter(); sprite_set_pos(shooter1, 100, 100);
        for(double angle = 0; angle <= 6.283185; angle += 0.006283185){
             sprite_set_angle(shooter1, angle);
             //graph_paint_screen(0x777777);
             graph_clear_screen();
             sprite_draw(shooter1);
             graph_draw();
        }
        t = clock() - t; //printf("%d\n", CLOCKS_PER_SEC);
        //double dt = ((double)t)/(double)CLOCKS_PER_SEC;
        printf("Time taken: %d/%d \n", t, CLOCKS_PER_SEC);
        sprite_dtor(shooter1);
    #endif

    #ifdef TELMO
    sprite_t *crosshair = get_crosshair();
    graph_clear_screen();
    sprite_draw(crosshair);
    graph_draw();
    #endif

    /// loop stuff
    int ipc_status;
    message msg;
    int good = 1;

    #ifdef DIOGO
        good = 0;
    #endif

    while (good) {
        /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    for (uint32_t i = 0, n = 1; i < 32; i++, n <<= 1) {
                        if (msg.m_notify.interrupts & n) {
                            interrupt_handler(i);
                            if ((scancode[0]) == ESC_BREAK_CODE) good = 0;
                            #ifdef TELMO
                            if (counter_mouse_ih >= 3) {
                                struct packet pp = mouse_parse_packet(packet_mouse_ih);
                                update_mouse_position(&pp);
                                sprite_set_pos(crosshair, get_mouse_X(), get_mouse_Y());
                                printf("X: %d | Y: %d | XRES: %d | YRES: %d\n", get_mouse_X(), get_mouse_Y(), graph_get_XRes(), graph_get_YRes());
                                graph_clear_screen();
                                sprite_draw(crosshair);
                                graph_draw();
                                counter_mouse_ih = 0;
                            }
                            #endif
                        }
                    }

                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else { /* received standart message, not a notification */
            /* no standart message expected: do nothing */
        }

        switch (get_hor_movement()) {
            case LEFT:
                printf("GOING LEFT.\n");
                break;
            case RIGHT:
                printf("GOING RIGHT.\n");
                break;
        }
        switch (get_ver_movement()) {
            case UP:
                printf("GOING UP.\n");
                break;
            case DOWN:
                printf("GOING DOWN.\n");
                break;
        }
    }

    // Unsubscribe interrupts
    if (unsubscribe_all()) {
        if (cleanup())
            printf("%s: failed to cleanup.\n", __func__);
        return 1;
    }


    if (cleanup()) {
        printf("%s: failed to cleanup.\n", __func__);
        return 1;
    }

    return 0;
}

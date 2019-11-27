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

#ifdef DIOGO
    #include "shooter.h"
    #include "pistol.xpm"
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
    if(graph_init(GRAPH_MODE)){
        printf("%s: failed to initalize graphics.\n", __func__);
        if (cleanup()) printf("%s: failed to cleanup.\n", __func__);
        return 1;
    }

    #ifdef DIOGO
        graph_paint_screen(0x777777);
        sprite_t *shooter1 = get_shooter(); sprite_set_pos(shooter1, 100, 100);
        for(double angle = 0; angle < 6.29; angle += 0.01){
             sprite_set_angle(shooter1, angle);
             //paint_screen(0x777777);
             sprite_draw(shooter1);
             tickdelay(micros_to_ticks(10000));
        }
        sprite_dtor(shooter1);
        graph_draw();
    #endif

    /// loop stuff
    int ipc_status, r;
    message msg;

    /// subscribe interrupts
    if (subscribe_all()) {
        if (cleanup())
            printf("%s: failed to cleanup.\n", __func__);
        return 1;
    }

    /// cycle
    int good = 1;
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
                        }
                    }
                    if (scancode[0] == ESC_BREAK_CODE) good = 0;
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else { /* received standart message, not a notification */
            /* no standart message expected: do nothing */
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

#include <lcom/lcf.h>
#include <lcom/proj.h>
#include <lcom/liblm.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8254.h"
#include "kbc_macros.h"
#include "graphics_macros.h"
#include "mouse_macros.h"
#include "proj_macros.h"
#include "errors.h"

#include "sprite.h"
#include "kbc.h"
#include "graphics.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "utils.h"
#include "interrupts_func.h"
#include "proj_func.h"

#ifdef DIOGO
    #include "plus.xpm"
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

    if (vbe_get_mode_information(GRAPH_MODE)) {
        printf("%s: failed to get information for mode %x.\n", __func__, GRAPH_MODE);
        if (cleanup())
            printf("%s: failed to cleanup.\n", __func__);
        return 1;
    }

    map_vram(); // if function fails it aborts program

    if (set_graphics_mode(GRAPH_MODE)) {
        printf("%s: failed to set graphic mode %x.\n", __func__, GRAPH_MODE);
        if (cleanup())
            printf("%s: failed to cleanup.\n", __func__);
        return 1;
    };

    #ifdef DIOGO
        sprite_t *sp = sprite_ctor((xpm_map_t)plus_xpm);
        sprite_draw(sp);
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

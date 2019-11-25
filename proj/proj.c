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

#include "kbc.h"
#include "graphics.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "utils.h"

int main(int argc, char* argv[]) {

    lcf_set_language("EN-US");

    //lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

    //lcf_log_output("/home/lcom/labs/proj/output.txt");

    if (lcf_start(argc, argv)) return 1;

    lcf_cleanup();

    return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {

    /// loop stuff
    int ipc_status, r;
    message msg;
    /// Keyboard interrupt handling
    uint8_t kbc_irq_bit = KBC_IRQ;
    int kbc_id = 0;
    int kbc_irq = BIT(kbc_irq_bit);
    if (subscribe_kbc_interrupt(kbc_irq_bit, &kbc_id)) {
        return 1;
    }

    /// Timer interrupt handling
    //const uint32_t frequency = sys_hz(); // Frequency asummed at 60Hz
    uint8_t timer_irq_bit = TIMER0_IRQ;
    int timer_id = 0;
    int timer_irq = BIT(timer_irq_bit);
    if(subscribe_timer_interrupt(timer_irq_bit, &timer_id)) {
        if (vg_exit()) printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        if (free_memory_map()) {
            printf("%s: lm_free failed\n", __func__);
        }
        return 1;
    }

    /// Mouse interrupt handling
    uint8_t mouse_irq_bit = MOUSE_IRQ;
    int mouse_id = 0;
    int mouse_irq = BIT(mouse_irq_bit);

    if (subscribe_mouse_interrupt(mouse_irq_bit, &mouse_id)) return 1; // subscribes mouse interrupts in exclusive mode
    if (sys_irqdisable(&mouse_id)) return 1; // temporarily disables our interrupts notifications
    if (mouse_set_data_report(true)) return 1; // enables mouse data reporting
    if (sys_irqenable(&mouse_id)) return 1; // re-enables our interrupts notifications

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
                    if (msg.m_notify.interrupts & kbc_irq) { /* subscribed interrupt */
                        kbc_ih();
                        if (scancode[0] == ESC_BREAK_CODE) good = 0;
                    }
                    if (msg.m_notify.interrupts & timer_irq) { /* subscribed interrupt */
                        timer_int_handler();
                    }

                    if (msg.m_notify.interrupts & mouse_irq) { /* subscribed interrupt */
                        mouse_ih();
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else { /* received standart message, not a notification */
            /* no standart message expected: do nothing */
        }
    }

    // Unsubscribe Keyboard interrupts
    if (unsubscribe_interrupt(&kbc_id)) {
        return 1;
    };

    // Unsubscribe Timer Interrupts
    if (unsubscribe_interrupt(&timer_id)) {
        return 1;
    }

    // Unsubscribe Mouse Interrupts
    if (sys_irqdisable(&mouse_id)) return 1; // temporarily disables our interrupts notifications
    if (mouse_set_data_report(false)) return 1; // enables mouse data reporting
    if (sys_irqenable(&mouse_id)) return 1; // re-enables our interrupts notifications
    if (unsubscribe_interrupt(&mouse_id)) return 1; // unsubscribes interrupts

    return 0;
}

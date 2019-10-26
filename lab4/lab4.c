#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse_func.h"
#include "kbc_func.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

extern uint8_t packet[3];
extern int counter;

int (mouse_test_packet)(uint32_t cnt) {
    /// loop stuff
    int ipc_status, r;
    message msg;
    /// Keyboard interrupt handling
    uint8_t mouse_irq_bit = 12;
    int mouse_id = 0;
    int mouse_irq = BIT(mouse_irq_bit);
    if (subscribe_mouse_interrupt(mouse_irq_bit, &mouse_id)) return 1;
    if(mouse_enable_data_reporting()) return 1;
    /// cycle
    int good = 1;
    uint32_t cnt_now = 0;
    while (good) {
        /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("driver_receive failed with %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE: /* hardware interrupt notification */
                    if (msg.m_notify.interrupts & mouse_irq) { /* subscribed interrupt */
                        mouse_ih();
                        if(counter >= 3){
                            struct packet pp = mouse_parse_packet(packet);
                            mouse_print_packet(&pp);
                            cnt_now++;
                            if(cnt == cnt_now) good = 0;
                        }
                        /*
                        if (!(two_byte_scancode || got_error)) { // finished processing a scancode
                            if (scancode[0] == TWO_BYTE_CODE) kbd_print_scancode(!(scancode[1] & BREAK_CODE_BIT), 2, scancode);
                            else                              kbd_print_scancode(!(scancode[0] & BREAK_CODE_BIT), 1, scancode);
                        } else { break; }
                        if (scancode[0] == ESC_BREAK_CODE) good = 0;
                        */
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else { /* received standart message, not a notification */
            /* no standart message expected: do nothing */
        }
    }

    if (unsubscribe_interrupt(&mouse_id)) return 1;
    if(kbc_restore_kbd()) return 1;

    return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* To be completed */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

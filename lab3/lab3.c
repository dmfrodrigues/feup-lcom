#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "kbc.h"
#include "kbc_func.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

extern uint8_t scancode[2];
extern int two_byte_scancode;
extern int got_error;
extern uint32_t sys_inb_counter;

int(kbd_test_scan)() {

    int ipc_status, r;
    message msg;

    uint8_t kbc_irq_bit = 1;
    int kbc_id = 0;
    int kbc_irq = BIT(kbc_irq_bit);

    int got_esc_breakcode = 0;

    if (subscribe_kbc_interrupt(kbc_irq_bit, &kbc_id)) return 1;

    while (!got_esc_breakcode) {
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

                        if (!(two_byte_scancode || got_error)) { /* finished processing a scancode */
                            if (scancode[0] == TWO_BYTE_CODE)
                                kbd_print_scancode(!(scancode[1] & BREAK_CODE_BIT), 2, scancode);
                            else
                                kbd_print_scancode(!(scancode[0] & BREAK_CODE_BIT), 1, scancode);
                        } else {
                            break;
                        }

                        if (scancode[0] == ESC_BREAK_CODE)
                            got_esc_breakcode = 1;

                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else { /* received standart message, not a notification */
            /* no standart message expected: do nothing */
        }
    }

    if (unsubscribe_kbc_interrupt(&kbc_id)) return 1;

    if (kbd_print_no_sysinb(sys_inb_counter)) return 1;

    return 0;
}

int(kbd_test_poll)() {
    uint8_t c[2]; uint8_t size;
    do{
        if(kbd_poll(c, &size)) return 1;
        if(kbd_print_scancode((~c[0])&BREAK_CODE_BIT, size, c)) return 1;
    }while(!(size == 1 && c[0] == ESC_BREAK_CODE));

    uint8_t cmd = 0;
    if(kbc_read_cmd(&cmd)) return 1;
    cmd = (cmd | INT_KBD) & (~DIS_KBD);
    if(kbc_change_cmd(cmd)) return 1;

    if (kbd_print_no_sysinb(sys_inb_counter)) return 1;

    return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

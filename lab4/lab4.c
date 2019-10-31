#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse.h"
#include "kbc.h"
#include "errors.h"
#include "mouse_macros.h"
#include "timer.h"

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
    int ret = 0;
    /// loop stuff
    int ipc_status, r;
    message msg;
    /// Mouse interrupt handling
    uint8_t mouse_irq_bit = 12;
    int mouse_id = 0;
    int mouse_irq = BIT(mouse_irq_bit);
    if ((ret = mouse_set_data_report(true))) return ret;
    //if ((ret = mouse_enable_data_reporting())) return ret;

    if (subscribe_mouse_interrupt(mouse_irq_bit, &mouse_id)) return 1;
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
    if (mouse_set_data_report(false)) return 1;
    return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {

    // Mouse packets data
    uint8_t packet[3];
    int sz = 0;
    uint8_t data = 0;
    // Cycle
    int packetCounter = 0;
    int good = 1;
    // return value
    int ret;

    while (good) {

        if ((ret = mouse_read_data(&data))) return ret;

        if ((data & FIRST_BYTE_ID) || sz) {
            packet[sz] = data;
            sz++;
        }

        if (sz == 3) {
            struct packet pp = mouse_parse_packet(packet);
            mouse_print_packet(&pp);
            packetCounter++;
            sz = 0;
            if (packetCounter == cnt) good = 0;
        }

        tickdelay(micros_to_ticks(period*1e3));
    }

    // Set Stream mode
    if ((ret = mouse_issue_cmd(SET_STREAM_MD))) return ret;
    // Disable data reporting
    if ((ret = mouse_set_data_report(false))) return ret;

    uint8_t cmd_byte = minix_get_dflt_kbc_cmd_byte();
    if ((ret = kbc_change_cmd(cmd_byte))) return ret;

    return SUCCESS;
}

int (mouse_test_async)(uint8_t idle_time) {
    /// loop stuff
    int ipc_status, r;
    message msg;
    /// Timer interrupt handling
    const uint32_t frequency = sys_hz(); // Frequency asummed at 60Hz
    uint8_t timer_irq_bit = 0;
    int timer_id = 0;
    int timer_irq = BIT(timer_irq_bit);
    if(subscribe_timer_interrupt(timer_irq_bit, &timer_id)) return 1;

    no_interrupts = 0;
    int time = 0;
    /// Mouse interrupt handling
    uint8_t mouse_irq_bit = 12;
    int mouse_id = 0;
    int mouse_irq = BIT(mouse_irq_bit);
    if (mouse_set_data_report(true)) return 1;

    if (subscribe_mouse_interrupt(mouse_irq_bit, &mouse_id)) return 1;
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
                    if (msg.m_notify.interrupts & timer_irq) { /* subscribed interrupt */
                        timer_int_handler();
                        if (no_interrupts%frequency == 0) time++;
                        if(time >= idle_time) good = 0;
                    }
                    if (msg.m_notify.interrupts & mouse_irq) { /// subscribed interrupt
                        mouse_ih();
                        if(counter >= 3){
                            struct packet pp = mouse_parse_packet(packet);
                            mouse_print_packet(&pp);
                            time = 0;
                            no_interrupts = 0;
                        }
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
    if (mouse_set_data_report(false)) return 1;

    if (unsubscribe_interrupt(&timer_id)) return 1;

    return 0;
}

enum state{
    STATE_NEUTRAL,
    STATE_FIRST,
    STATE_MID,
    STATE_SECOND
};

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    int ret = 0;
    /// loop stuff
    int ipc_status, r;
    message msg;
    /// Mouse interrupt handling
    uint8_t mouse_irq_bit = 12;
    int mouse_id = 0;
    int mouse_irq = BIT(mouse_irq_bit);
    //if ((ret = mouse_set_data_report(true))) return ret;
    if ((ret = mouse_enable_data_reporting())) return ret;

    if (subscribe_mouse_interrupt(mouse_irq_bit, &mouse_id)) return 1;
    /// cycle
    int good = 1;
    //int st = STATE_NEUTRAL;
    //int16_t dx, dy;
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
                            mouse_print_packet(&pp);/*
                            switch(st){
                                case STATE_NEUTRAL:
                                    if(pp.lb == 1 && pp.mb == 0 && pp.rb == 0){
                                        st = STATE_FIRST;
                                        dx = 0;
                                        dy = 0;
                                    }
                                    break;
                                case STATE_FIRST:
                                    if(pp.lb == 1){
                                        if(pp.delta_x >= -tolerance && pp.delta_y >= -tolerance){
                                            dx += pp.delta_x;
                                            dy += pp.delta_y;
                                        }else{
                                            st = STATE_NEUTRAL;
                                        }
                                    }else{
                                        if(abs(dx) < abs(dy) && dx > 0 && dy > 0){
                                            st = STATE_MID;
                                        }else{
                                            st = STATE_NEUTRAL;
                                        }
                                    }
                                    break;
                                case STATE_MID:
                                    if(pp.lb == 0 && pp.mb == 0 && pp.rb == 1){
                                        st = STATE_SECOND;
                                        dx = 0;
                                        dy = 0;
                                    }else{
                                        st = STATE_NEUTRAL;
                                    }
                                    break;
                                case STATE_SECOND:
                                    if(pp.rb == 1){
                                        if(pp.delta_x >= -tolerance && pp.delta_y <= tolerance){
                                            dx += pp.delta_x;
                                            dy += pp.delta_y;
                                        }else{
                                            st = STATE_NEUTRAL;
                                        }
                                    }else{
                                        if(abs(dx) < abs(dy) && dx > 0 && dy < 0){
                                            st = STATE_NEUTRAL;
                                            good = false;
                                        }else{
                                            st = STATE_NEUTRAL;
                                        }
                                    }
                                    break;
                                default:
                                    return OTHER_ERROR;
                                    break;
                            }*/
                        }
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
    if (mouse_set_data_report(false)) return 1;

    return 0;
}

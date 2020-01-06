#include <lcom/lcf.h>

#include "interrupts_func.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "hltp.h"
#include "utils.h"
#include "errors.h"

static int timer_subscribed = 0, timer_id;

static int keyboard_subscribed = 0, kbc_id;

static int mouse_subscribed = 0, mouse_id;

static int uart_subscribed = 0, uart_id;

static void (*const ih[])(void) =   {    timer_int_handler,
                                         kbc_ih,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         mouse_ih,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                     };
/*
static void process_received(const uint8_t *p, const size_t sz){
    void *q = NULL;
    hltp_type t = hltp_interpret(p, sz, &q);
    switch(t){
        case hltp_type_string:{
            char *s = q;
            printf("%s\n", s);
        } break;
        default: break;
    }
}
*/
int (subscribe_all)(void) {

    /// Timer interrupt handling
    timer_id = 0;
    if(subscribe_timer_interrupt(TIMER0_IRQ, &timer_id)) {
        printf("%s: failed to subscribe timer interrupts.\n", __func__);
        return SBCR_ERROR;
    }
    timer_subscribed = 1;

    /// Keyboard interrupt handling
    kbc_id = 0;
    if (subscribe_kbc_interrupt(KBC_IRQ, &kbc_id)) {
        printf("%s: failed to subscribe keyboard interrupts.\n", __func__);
        if (unsubscribe_all())
            printf("%s: failed to unsubcribe driver, unexpected behaviour is expected.\n", __func__);
        return SBCR_ERROR;
    }
    keyboard_subscribed = 1;

    /// Mouse interrupt handling
    mouse_id = 0;
    if (subscribe_mouse_interrupt(MOUSE_IRQ, &mouse_id)) { // subscribes mouse interrupts in exclusive mode
        printf("%s: failed to subscribe mouse interrupts.\n", __func__);
        if (unsubscribe_all())
            printf("%s: failed to unsubcribe driver, unexpected behaviour is expected.\n", __func__);
        return SBCR_ERROR;
    }
    mouse_subscribed = 1;
    if (sys_irqdisable(&mouse_id)) { // temporarily disables our interrupts notifications
        printf("%s: failed to disable mouse interrupts.\n", __func__);
        if (unsubscribe_all())
            printf("%s: failed to unsubcribe driver, unexpected behaviour is expected.\n", __func__);
        return SBCR_ERROR;
    }
    if (mouse_set_data_report(true)) { // enables mouse data reporting
        printf("%s: failed to enable mouse data reporting.\n", __func__);
        if (unsubscribe_all())
            printf("%s: failed to unsubcribe driver, unexpected behaviour is expected.\n", __func__);
        return SBCR_ERROR;
    }
    if (sys_irqenable(&mouse_id)) { // re-enables our interrupts notifications
        printf("%s: failed to enable mouse interrupts.\n", __func__);
        if (unsubscribe_all())
            printf("%s: failed to unsubcribe driver, unexpected behaviour is expected.\n", __func__);
        return SBCR_ERROR;
    }

    /// UART interrupt handling
    uart_id = 0;
    uart_set_bits_per_character(COM1_ADDR, 8);
    uart_set_stop_bits         (COM1_ADDR, 2);
    uart_set_parity            (COM1_ADDR, uart_parity_even);
    uart_set_bit_rate          (COM1_ADDR, 19200);
    uart_enable_int_rx (COM1_ADDR);
    uart_enable_int_tx(COM1_ADDR);
    if(subscribe_uart_interrupt(COM1_IRQ, &uart_id)) {
        printf("%s: failed to subscribe UART interrupts.\n", __func__);
        return SBCR_ERROR;
    }
    uart_subscribed = 1;
    nctp_init();

    return SUCCESS;
}

int (unsubscribe_all)(void) {
    int r = SUCCESS;

    // Unsubscribe Timer Interrupts
    if (timer_subscribed) {
        if (unsubscribe_interrupt(&timer_id)) {
            printf("%s: failed to unsubcribe timer interrupts.\n", __func__);
            r = UNSBCR_ERROR;
        }
        timer_subscribed = 0;
    }

    // Unsubscribe Keyboard interrupts
    if (keyboard_subscribed) {
        if (unsubscribe_interrupt(&kbc_id)) {
            printf("%s: failed to unsubcribe keyboard interrupts.\n", __func__);
            r = UNSBCR_ERROR;
        }
        keyboard_subscribed = 0;
    }

    // Unsubscribe Mouse Interrupts
    if (mouse_subscribed) {
        if (sys_irqdisable(&mouse_id)) { // temporarily disables our interrupts notifications
            printf("%s: failed to disable mouse interrupts.\n", __func__);
            r = UNSBCR_ERROR;
        }
        if (mouse_set_data_report(false)) { // disables mouse data reporting
            printf("%s: failed to disable mouse data reporting.\n", __func__);
            r = UNSBCR_ERROR;
        }
        if (sys_irqenable(&mouse_id)) { // re-enables our interrupts notifications
            printf("%s: failed to enable mouse interrupts.\n", __func__);
            r = UNSBCR_ERROR;
        }
        if (unsubscribe_interrupt(&mouse_id)) { // unsubscribes interrupts
            printf("%s: failed to unsubcribe mouse interrupts.\n", __func__);
            r = UNSBCR_ERROR;
        }
        mouse_subscribed = 0;
    }

    // Unsubscribe UART interrupts
    if (uart_subscribed) {
        if (unsubscribe_interrupt(&uart_id)) {
            printf("%s: failed to unsubcribe UART interrupts.\n", __func__);
            r = UNSBCR_ERROR;
        }
        uart_enable_int_rx(COM1_ADDR);
        uart_enable_int_tx(COM1_ADDR);
        uart_subscribed = 0;
    }
    nctp_free();

    return r;
}

void (interrupt_handler)(uint8_t handler) {
    if (handler >= 32)              return;
    if ((ih[handler]) == NULL)      return;
    (*ih[handler])();
}

int get_interrupts_vector(uint64_t *p){
    int r;

    *p = 0;

    int ipc_status;
    message msg;
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
        printf("driver_receive failed with %d", r);
        return OTHER_ERROR;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */
                *p = msg.m_notify.interrupts;
                return SUCCESS;
            default: break; /* no other notifications expected: do nothing */
        }
    }
    return SUCCESS;
}

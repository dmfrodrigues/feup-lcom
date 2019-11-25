#include "interrupts_func.h"
#include "timer.h"
#include "i8254.h"
#include "kbc_macros.h"
#include "keyboard.h"
#include "mouse.h"
#include "utils.h"
#include "errors.h"

#include <lcom/lcf.h>

static int timer_subscribed = 0, timer_id;

static int keyboard_subscribed = 0, kbc_id;

static int mouse_subscribed = 0, mouse_id;

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
            printf("%s: failed to unsubcribe driver, unexpected behaviour is expected.\n");
        return SBCR_ERROR;
    }
    keyboard_subscribed = 1;

    /// Mouse interrupt handling
    mouse_id = 0;
    if (subscribe_mouse_interrupt(MOUSE_IRQ, &mouse_id)) { // subscribes mouse interrupts in exclusive mode
        printf("%s: failed to subscribe mouse interrupts.\n", __func__);
        if (unsubscribe_all())
            printf("%s: failed to unsubcribe driver, unexpected behaviour is expected.\n");
        return SBCR_ERROR;
    }
    mouse_subscribed = 1;
    if (sys_irqdisable(&mouse_id)) { // temporarily disables our interrupts notifications
        printf("%s: failed to disable mouse interrupts.\n", __func__);
        if (unsubscribe_all())
            printf("%s: failed to unsubcribe driver, unexpected behaviour is expected.\n");
        return SBCR_ERROR;
    }
    if (mouse_set_data_report(true)) { // enables mouse data reporting
        printf("%s: failed to enable mouse data reporting.\n", __func__);
        if (unsubscribe_all())
            printf("%s: failed to unsubcribe driver, unexpected behaviour is expected.\n");
        return SBCR_ERROR;
    }
    if (sys_irqenable(&mouse_id)) { // re-enables our interrupts notifications
        printf("%s: failed to enable mouse interrupts.\n", __func__);
        if (unsubscribe_all())
            printf("%s: failed to unsubcribe driver, unexpected behaviour is expected.\n");
        return SBCR_ERROR;
    }

    return SUCCESS;
}

int (unsubscribe_all)(void) {
    int r = SUCCESS;

    // Unsubscribe Timer Interrupts
    if (timer_subscribed) {
        if (unsubscribe_interrupt(&timer_id)) {
            printf("%s: failed to unsubcribe timer interrupts.\n");
            r = UNSBCR_ERROR;
        }
        timer_subscribed = 0;
    }

    // Unsubscribe Keyboard interrupts
    if (keyboard_subscribed) {
        if (unsubscribe_interrupt(&kbc_id)) {
            printf("%s: failed to unsubcribe keyboard interrupts.\n");
            r = UNSBCR_ERROR;
        }
        keyboard_subscribed = 0;
    }

    // Unsubscribe Mouse Interrupts
    if (mouse_subscribed) {
        if (sys_irqdisable(&mouse_id)) { // temporarily disables our interrupts notifications
            printf("%s: failed to disable mouse interrupts.\n");
            r = UNSBCR_ERROR;
        }
        if (mouse_set_data_report(false)) { // disables mouse data reporting
            printf("%s: failed to disable mouse data reporting.\n");
            r = UNSBCR_ERROR;
        }
        if (sys_irqenable(&mouse_id)) { // re-enables our interrupts notifications
            printf("%s: failed to enable mouse interrupts.\n");
            r = UNSBCR_ERROR;
        }
        if (unsubscribe_interrupt(&mouse_id)) { // unsubscribes interrupts
            printf("%s: failed to unsubcribe mouse interrupts.\n");
            r = UNSBCR_ERROR;
        }
        mouse_subscribed = 0;
    }

    return r;
}

void interrupt_handler(uint8_t handler) {
    if (handler >= 32)              return;
    if ((*ih[handler]) == NULL)     return;
    (*ih[handler])();
}
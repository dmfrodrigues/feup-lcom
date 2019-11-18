#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "graphics.h"
#include "graphics_macros.h"
#include "keyboard.h"
#include "kbc.h"
#include "kbc_macros.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab5/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
    return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
    //int r;
    //if ((r = get_permissions_first_mbyte()))
    //    panic("%s: sys_privctl (ADD MEM) failed: %d\n", __func__, r);

    if (vbe_get_mode_information(mode)) {
        printf("%s: failed to get information for mode %x.\n", __func__, mode);
        if (vg_exit())
            printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        return 1;
    }

    map_vram(); // if function fails it aborts program

    if (set_graphics_mode(mode)) {
        printf("%s: failed to set graphic mode %x.\n", __func__, mode);
        if (vg_exit())
            printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        return 1;
    };

    tickdelay(micros_to_ticks(delay*1e6));

    if (vg_exit()) {
        printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        if (free_memory())
            printf("%s: lm_free failed\n", __func__);
        return 1;
    }

    if (free_memory()) {
        printf("%s: lm_free failed\n", __func__);
        return 1;
    }

    return 0;
}

// lcom_run lab5 "rectangle 105 100 100 100 100 1"
// lcom_run lab5 "rectangle 115 100 100 100 100 FF0000"
int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    int r;
    //if ((r = get_permissions_first_mbyte()))
    //    panic("%s: sys_privctl (ADD MEM) failed: %d\n", __func__, r);

    if (vbe_get_mode_information(mode)) {
        printf("%s: failed to get information for mode %x.\n", __func__, mode);
        if (vg_exit())
            printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        return 1;
    }

    map_vram(); // if function fails it aborts program

    if (set_graphics_mode(mode)) {
        printf("%s: failed to set graphic mode %x.\n", __func__, mode);
        if (vg_exit()) printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        return 1;
    };

    if (draw_rectangle(x, y, width, height, color)) {
        if (vg_exit()) {
            printf("%s: vg_exit failed to exit to text mode.\n", __func__);
            if (free_memory()) printf("%s: lm_free failed\n", __func__);
        }
        return 1;
    }

    /// loop stuff
    int ipc_status;
    message msg;
    /// Keyboard interrupt handling
    uint8_t kbc_irq_bit = KBC_IRQ;
    int kbc_id = 0;
    int kbc_irq = BIT(kbc_irq_bit);
    if (subscribe_kbc_interrupt(kbc_irq_bit, &kbc_id)) {
        if (vg_exit()) {
            printf("%s: vg_exit failed to exit to text mode.\n", __func__);
            if (free_memory()) printf("%s: lm_free failed\n", __func__);
        }
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
                    if (msg.m_notify.interrupts & kbc_irq) { /* subscribed interrupt */
                        kbc_ih();
                        if (scancode[0] == ESC_BREAK_CODE) good = 0;
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else { /* received standart message, not a notification */
            /* no standart message expected: do nothing */
        }
    }

    if (unsubscribe_interrupt(&kbc_id)) {
        if (vg_exit()) {
            printf("%s: vg_exit failed to exit to text mode.\n", __func__);
            if (free_memory()) printf("%s: lm_free failed\n", __func__);
        }
        return 1;
    };

    if (vg_exit()) {
        printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        if (free_memory()) printf("%s: lm_free failed\n", __func__);
        return 1;
    }

    if (free_memory()) {
        printf("%s: lm_free failed\n", __func__);
        return 1;
    }

    return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
    int r;
    //if ((r = get_permissions_first_mbyte()))
    //    panic("%s: sys_privctl (ADD MEM) failed: %d\n", __func__, r);

    if (vbe_get_mode_information(mode)) {
        printf("%s: failed to get information for mode %x.\n", __func__, mode);
        if (vg_exit())
            printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        return 1;
    }

    map_vram(); // if function fails it aborts program

    if (set_graphics_mode(mode)) {
        printf("%s: failed to set graphic mode %x.\n", __func__, mode);
        if (vg_exit()) printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        return 1;
    };

    uint16_t W = get_XRes()/no_rectangles;
    uint16_t H = get_YRes()/no_rectangles;

    uint32_t color, R, G, B;
    for(uint8_t row = 0; row < no_rectangles; ++row){
        for(uint8_t col = 0; col < no_rectangles; ++col){
            if(get_bytes_pixel() == 1){
                color = (first + (row * no_rectangles + col) * step) % (1 << get_bits_pixel());
            }else{
                R = (GET_RED(first) + col*step) % (1 << get_RedMaskSize());
                G = (GET_GRE(first) + row*step) % (1 << get_GreenMaskSize());
                B = (GET_BLU(first) + (col+row)*step) % (1 << get_BlueMaskSize());
                color = SET_COLOR(R,G,B);
            }
            if (draw_rectangle(col*W,row*H,W,H,color)) {
                if (vg_exit()) {
                    printf("%s: vg_exit failed to exit to text mode.\n", __func__);
                    if (free_memory()) printf("%s: lm_free failed\n", __func__);
                }
                return 1;
            }
        }
    }
    /// loop stuff
    int ipc_status;
    message msg;
    /// Keyboard interrupt handling
    uint8_t kbc_irq_bit = KBC_IRQ;
    int kbc_id = 0;
    int kbc_irq = BIT(kbc_irq_bit);
    if (subscribe_kbc_interrupt(kbc_irq_bit, &kbc_id)) {
        if (vg_exit()) {
            printf("%s: vg_exit failed to exit to text mode.\n", __func__);
            if (free_memory()) printf("%s: lm_free failed\n", __func__);
        }
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
                    if (msg.m_notify.interrupts & kbc_irq) { /* subscribed interrupt */
                        kbc_ih();
                        if (scancode[0] == ESC_BREAK_CODE) good = 0;
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else { /* received standart message, not a notification */
            /* no standart message expected: do nothing */
        }
    }

    if (unsubscribe_interrupt(&kbc_id)) {
        if (vg_exit()) {
            printf("%s: vg_exit failed to exit to text mode.\n", __func__);
            if (free_memory()) printf("%s: lm_free failed\n", __func__);
        }
        return 1;
    };

    if (vg_exit()) {
        printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        if (free_memory()) printf("%s: lm_free failed\n", __func__);
        return 1;
    }

    if (free_memory()) {
        printf("%s: lm_free failed\n", __func__);
        return 1;
    }

    return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    int r;
    if ((r = get_permissions_first_mbyte()))
        panic("%s: sys_privctl (ADD MEM) failed: %d\n", __func__, r);

    if (vbe_get_mode_information(INDEXED_1024_768)) {
        printf("%s: failed to get information for mode %x.\n", __func__, INDEXED_1024_768);
        if (vg_exit())
            printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        return 1;
    }

    map_vram(); // if function fails it aborts program

    if (set_graphics_mode(INDEXED_1024_768)) {
        printf("%s: failed to set graphic mode %x.\n", __func__, INDEXED_1024_768);
        if (vg_exit()) printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        return 1;
    };

    enum xpm_image_type type = XPM_INDEXED;
    xpm_image_t img;

    uint8_t *map = xpm_load(xpm, type, &img);

    for (int i = 0; i < img.width; i++) {
        for (int j = 0; j < img.height; j++) {
            set_pixel(x + i, y + j, map[i + j * img.width]);
        }
    }

    /// loop stuff
    int ipc_status;
    message msg;
    /// Keyboard interrupt handling
    uint8_t kbc_irq_bit = KBC_IRQ;
    int kbc_id = 0;
    int kbc_irq = BIT(kbc_irq_bit);
    if (subscribe_kbc_interrupt(kbc_irq_bit, &kbc_id)) {
        if (vg_exit()) {
            printf("%s: vg_exit failed to exit to text mode.\n", __func__);
            if (free_memory()) printf("%s: lm_free failed\n", __func__);
        }
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
                    if (msg.m_notify.interrupts & kbc_irq) { /* subscribed interrupt */
                        kbc_ih();
                        if (scancode[0] == ESC_BREAK_CODE) good = 0;
                    }
                    break;
                default:
                    break; /* no other notifications expected: do nothing */
            }
        } else { /* received standart message, not a notification */
            /* no standart message expected: do nothing */
        }
    }

    if (unsubscribe_interrupt(&kbc_id)) {
        if (vg_exit()) {
            printf("%s: vg_exit failed to exit to text mode.\n", __func__);
            if (free_memory()) printf("%s: lm_free failed\n", __func__);
        }
        return 1;
    };

    if (vg_exit()) {
        printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        if (free_memory()) printf("%s: lm_free failed\n", __func__);
        return 1;
    }

    if (free_memory()) {
        printf("%s: lm_free failed\n", __func__);
        return 1;
    }

    return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {

    return 1;
}

int(video_test_controller)() {

    

    return 1;
}

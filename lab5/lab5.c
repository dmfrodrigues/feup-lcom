#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "graphics.h"
#include "graphics_macros.h"

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
    int r;
    if ((r = get_permissions_first_mbyte()))
        panic("%s: sys_privctl (ADD MEM) failed: %d\n", __func__, r);

    if (set_graphics_mode(mode)) {
        printf("%s: failed to set graphic mode %x.\n", __func__, mode);
        if (vg_exit())
            printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        return 1;
    };

    if (vbe_get_mode_information(mode)) {
        printf("%s: failed to get information for mode %x.\n", __func__, mode);
        if (vg_exit())
            printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        return 1;
    }

    map_vram(); // if function fails it aborts program

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

///lcom_run lab5 "rectangle 105 3 3 30 30 12 -t 1"
int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    int r;
    if ((r = get_permissions_first_mbyte()))
    panic("%s: sys_privctl (ADD MEM) failed: %d\n", __func__, r);

    if (set_graphics_mode(mode)) {
        printf("%s: failed to set graphic mode %x.\n", __func__, mode);
        if (vg_exit()) printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        return 1;
    };

    vg_draw_hline(x, y, width, color);
    tickdelay(micros_to_ticks(1000000));
    if (vg_exit()) {
        printf("%s: vg_exit failed to exit to text mode.\n", __func__);
        if (free_memory()) printf("%s: lm_free failed\n", __func__);
        return 1;
    }
    return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* To be completed */
  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__,
         mode, no_rectangles, first, step);

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    /* To be completed */
    printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

    return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
    /* To be completed */
    printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
    __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

    return 1;
}

int(video_test_controller)() {
    /* To be completed */
    printf("%s(): under construction\n", __func__);

    return 1;
}

#include "proj_func.h"
#include "interrupts_func.h"
#include "graphics.h"
#include "errors.h"

#include <lcom/lcf.h>

int cleanup(void) {
    int r = SUCCESS;
    if ((r = unsubscribe_all()))
        printf("%s: failed to unsubscribe drivers.\n", __func__);

    if ((r = vg_exit()))
        printf("%s: vg_exit failed to exit to text mode.\n", __func__);
    if ((r = free_memory_map()))
        printf("%s: lm_free failed\n", __func__);

    return r;
}

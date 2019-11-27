#include "proj_func.h"
#include "interrupts_func.h"
#include "graph.h"
#include "errors.h"

#include <lcom/lcf.h>

int cleanup(void) {
    int r = SUCCESS;
    if ((r = unsubscribe_all()))
        printf("%s: failed to unsubscribe drivers.\n", __func__);
    if ((r = graph_cleanup()))
        printf("%s: graph cleanup failed\n", __func__);

    return r;
}

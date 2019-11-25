#include <lcom/lcf.h>
#include <lcom/proj.h>
#include <lcom/liblm.h>

#include <stdint.h>

int main(int argc, char* argv[]) {

    lcf_set_language("EN-US");

    //lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

    //lcf_log_output("/home/lcom/labs/proj/output.txt");

    if (lcf_start(argc, argv)) return 1;

    lcf_cleanup();

    return 0;
}

#include <lcom/lcf.h>

#include "errors.h"
#include "nctp.h"

int ser_test_conf(unsigned short base_addr) {
	int ret = SUCCESS;
	uart_config conf;
	if((ret = uart_get_config(base_addr, &conf))) return ret;
	uart_print_config(conf);
	return SUCCESS;
}

int ser_test_set(unsigned short base_addr, unsigned long bits, unsigned long stop,
	           long parity, unsigned long rate) {
    int par;
   	switch(parity){
		case -1: par = uart_parity_none; break;
		case  0: par = uart_parity_even; break;
		case +1: par = uart_parity_odd ; break;
		default: return INVALID_ARG;
   	}
	int ret = SUCCESS;
	if((ret = uart_set_bits_per_character(base_addr, bits))) return ret; tickdelay(micros_to_ticks(100000));
	if((ret = uart_set_stop_bits         (base_addr, stop))) return ret; tickdelay(micros_to_ticks(100000));
	if((ret = uart_set_parity            (base_addr, par ))) return ret; tickdelay(micros_to_ticks(100000));
	if((ret = uart_set_bit_rate          (base_addr, rate))) return ret; tickdelay(micros_to_ticks(100000));

	return SUCCESS;
}

int ser_test_poll(unsigned short base_addr, unsigned char tx, unsigned long bits,
                    unsigned long stop, long parity, unsigned long rate,
                    int stringc, char *strings[]) {
	int ret = SUCCESS;
    if((ret = ser_test_set(base_addr, bits, stop, parity, rate))) return ret;
	if((ret = uart_disable_int_rx(base_addr))) return ret;
	if((ret = uart_disable_int_tx(base_addr))) return ret;
	if(tx == 0){
		uint8_t c = ' ';
		int r;
		while(c != '.'){
			r = nctp_get_char_poll(base_addr, &c);
			if(r == TIMEOUT_ERROR) continue;
			else if(r != SUCCESS ) return r;
			printf("%c", c);
		}
	}else{
		for(int i = 0; i < stringc; ++i){
			for(int j = 0; strings[i][j] != 0; ++j)
				if((ret = nctp_send_char_poll(base_addr, strings[i][j])))
					return ret;
			if(i+1 != stringc)
				if((ret = nctp_send_char_poll(base_addr, ' '))) return ret;
		}
		if((ret = nctp_send_char_poll(base_addr, '.'))) return ret;
	}
	return SUCCESS;
}

int ser_test_int(/* details to be provided */) {
    /* To be completed */
	return 1;
}

int ser_test_fifo(/* details to be provided */) {
    /* To be completed */
	return 1;
}

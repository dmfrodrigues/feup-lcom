PROG=proj

.PATH: ${.CURDIR}/src
.PATH: ${.CURDIR}/libs/peripherals/src
.PATH: ${.CURDIR}/libs/classes/src

SRCS= proj.c list.c queue.c graph.c kbc.c keyboard.c mouse.c utils.c timer.c interrupts_func.c proj_func.c fast_math.c rectangle.c font.c xpm_utils.c ent.c rtc.c uart.c hltp.c makecode_map.c
IPATHS=-I./include -I./libs/peripherals/include -I./libs/classes/include -I./maps -I./media/xpm

CPPFLAGS += -pedantic ${IPATHS} -D LCOM_MACRO -D DIOGO #-D __LCOM_OPTIMIZED_

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>

PROG=proj

.PATH: ${.CURDIR}/src
.PATH: ${.CURDIR}/libs/graph/src
.PATH: ${.CURDIR}/libs/kbc/src
.PATH: ${.CURDIR}/libs/rtc/src
.PATH: ${.CURDIR}/libs/timer/src
.PATH: ${.CURDIR}/libs/uart/src
.PATH: ${.CURDIR}/libs/classes/src
.PATH: ${.CURDIR}/libs/utils/src

SRCS= list.c queue.c graph.c kbc.c keyboard.c mouse.c utils.c timer.c interrupts_func.c fast_math.c xpm_utils.c ent.c rtc.c uart.c hltp.c makecode_map.c menu.c proj.c proj_func.c rectangle.c font.c
IPATHS=-I./include -I./libs/graph/include -I./libs/kbc/include -I./libs/rtc/include -I./libs/timer/include -I./libs/uart/include -I./libs/classes/include -I./libs/utils/include -I./maps -I./media/xpm

CPPFLAGS += -pedantic ${IPATHS} -D LCOM_MACRO -D __LCOM_OPTIMIZED__ #-Weverything


DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>

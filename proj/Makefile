PROG=proj

.PATH: ${.CURDIR}/project/src
.PATH: ${.CURDIR}/libs/graph/src
.PATH: ${.CURDIR}/libs/kbc/src
.PATH: ${.CURDIR}/libs/rtc/src
.PATH: ${.CURDIR}/libs/timer/src
.PATH: ${.CURDIR}/libs/uart/src
.PATH: ${.CURDIR}/libs/classes/src
.PATH: ${.CURDIR}/libs/utils/src

SRCS= list.c queue.c graph.c kbc.c keyboard.c mouse.c utils.c timer.c interrupts_func.c fast_math.c xpm_utils.c rtc.c uart.c makecode_map.c menu.c proj_func.c rectangle.c font.c ent.c proj.c hltp.c zombies.c campaign.c singleplayer.c chat.c scoreboards.c
IPATHS=-I./project/include -I./libs -I./libs/graph/include -I./libs/kbc/include -I./libs/rtc/include -I./libs/timer/include -I./libs/uart/include -I./libs/classes/include -I./libs/utils/include -I./maps -I./media/xpm

CPPFLAGS += -pedantic ${IPATHS} -D LCOM_MACRO -D __LCOM_OPTIMIZED__ -Wall -Wextra -Wshadow -Wunreachable-code #-Weverything -Wno-padded -Wno-unused-macros

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>

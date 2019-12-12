PROG=proj

.PATH: ${.CURDIR}/src

SRCS= proj.c graph.c kbc.c keyboard.c mouse.c utils.c timer.c interrupts_func.c proj_func.c fast_math.c rectangle.c font.c xpm_utils.c ent.c

CPPFLAGS += -pedantic -I./include -I./maps -I./media/xpm -D LCOM_MACRO -D TELMO #-D __LCOM_OPTIMIZED_

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>

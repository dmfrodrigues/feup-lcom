PROG=proj

.PATH: ${.CURDIR}/src

SRCS= proj.c graph.c sprite.c kbc.c keyboard.c mouse.c utils.c timer.c interrupts_func.c proj_func.c fast_math.c

CPPFLAGS += -pedantic -I./include -I./bmp -I./xpm -D TELMO #-D __LCOM_OPTIMIZED_

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>

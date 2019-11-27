PROG=proj

SRCS= proj.c graph.c sprite.c kbc.c keyboard.c mouse.c utils.c timer.c interrupts_func.c proj_func.c

CPPFLAGS += -pedantic -I./bmp -I./xpm -D __LCOM_OPTIMIZED_

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>

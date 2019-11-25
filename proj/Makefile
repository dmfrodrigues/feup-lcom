PROG=proj

SRCS= proj.c graphics.c sprite.c kbc.c keyboard.c mouse.c utils.c timer.c interrupts_func.c

CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED_

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>

PROG=proj

.PATH: ${.CURDIR}/src
.PATH: ${.CURDIR}/../lab2/src
.PATH: ${.CURDIR}/../lab3/src
.PATH: ${.CURDIR}/../lab4/src
.PATH: ${.CURDIR}/../lab5/src

SRCS= proj.c graph.c sprite.c kbc.c keyboard.c mouse.c utils.c timer.c interrupts_func.c proj_func.c fast_math.c rectangle.c font.c xpm_utils.c ent.c

CPPFLAGS += -pedantic -I./include -I../lab2/include -I./bmp -I./xpm -D LCOM_MACRO -D __LCOM_OPTIMIZED_

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>

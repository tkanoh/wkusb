PROG=	wkusb
SRCS=	wkusb.c
BINDIR=	/usr/local/bin
MKMAN=	no
CLEANFILES=	wkusb.core

# tty device
CFLAGS+=	-DTTY_DEV=\"/dev/ttyU4\"

.include <bsd.prog.mk>

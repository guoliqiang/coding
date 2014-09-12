# Dlib makefile
MAKE = make
VERSION = @ver@
SRCDIR = src/
DESTDIR = 
CC = gcc
CFLAGS = -g -O2 -Wall -W -I/home/guoliqiang/git-coding/coding/third_part/email/email-3.1.3/dlib -I/home/guoliqiang/git-coding/coding/third_part/email/email-3.1.3/dlib/src -I/home/guoliqiang/git-coding/coding/third_part/email/email-3.1.3/dlib/include -I../include  -DHAVE_CONFIG_H -DHAVE_LIBSSL
LDFLAGS = -L/home/guoliqiang/git-coding/coding/third_part/openssl/lib
LIBS = -lnsl -lm  -lssl -crypto

prefix = /usr/local
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
mandir = ${datarootdir}/man
docdir = ${datarootdir}/doc/${PACKAGE_TARNAME}
sysconfdir = ${prefix}/etc
datarootdir = ${prefix}/share

bin_suffix = 

FILES = src/dnet.o src/dhash.o src/dlist.o src/dstrbuf.o src/dutil.o src/dvector.o

.PHONY: all clean-all clean distclean install uninstall

all: $(FILES)
	ar rc libdlib.a $(FILES)

test:
	$(CC) $(CFLAGS) -o test test.c libdlib.a -lm

dlisttest:
	$(CC) $(CFLAGS) -o dlisttest dlisttest.c libdlib.a -lm

dhashtest:
	$(CC) $(CFLAGS) -o dhashtest dhashtest.c libdlib.a -lm

clean:
	rm -f src/*.o *.so *.a test


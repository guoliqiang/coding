MAKE = make
VERSION = @VERSION@
SRCDIR = .
CC = gcc
CFLAGS = -g -O2 -Wall -W -DUSE_GNU_STRFTIME -I/home/guoliqiang/git-coding/coding/third_part/email/email-3.1.3 -I/home/guoliqiang/git-coding/coding/third_part/email/email-3.1.3/src -I/home/guoliqiang/git-coding/coding/third_part/email/email-3.1.3/include -I../include -I/home/guoliqiang/git-coding/coding/third_part/email/email-3.1.3/dlib/include -I../dlib/include -DEMAIL_VERSION='"3.1.3"' -DCOMPILE_DATE='"10/31/2010-01:13:35PM_EDT"' -DEMAIL_DIR='"${prefix}/etc/email"' -DHAVE_CONFIG_H -DHAVE_LIBSSL
LDFLAGS = -L/home/guoliqiang/git-coding/coding/third_part/openssl/lib
LIBS = -lm  -lssl -lcrypto -ldl
DLIB = ../dlib/libdlib.a

prefix = /home/guoliqiang/git-coding/coding/third_part/email/
bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
mandir = ${datarootdir}/man
sysconfdir = ${prefix}/etc
datarootdir = ${prefix}/share

FILES = email.o addr_parse.o addy_book.o conf.o error.o execgpg.o file_io.o \
        message.o mimeutils.o processmail.o progress_bar.o \
	remotesmtp.o sig_file.o smtpcommands.o utils.o

all: $(FILES)
	$(CC) $(CFLAGS) -o email $(FILES) $(OTHER_FILES) $(DLIB) $(LDFLAGS) $(LIBS)

clean:
	rm -f *.o *.d email

clean-all:
	rm -rf Makefile *.o *.d email


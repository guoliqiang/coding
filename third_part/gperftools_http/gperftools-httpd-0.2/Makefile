
CC=gcc
CXX=g++
CFLAGS=-O2 -fpic -ggdb  -I/home/guoliqiang/git-coding/coding/third_part/gperftools/include/


.c.o:
	@rm -f $@
	$(CC) $(CFLAGS) -c $*.c

.cc.o:
	@rm -f $@
	$(CXX) $(CFLAGS) -c $*.cc

OFILES = thttpd.o libhttpd.o fdwatch.o timers.o pprof.o 

ALL = libghttpd.a libghttpd.so libghttpd-preload.so

CLEANFILES =	$(ALL) *.o	

all:		$(ALL)

libghttpd.a: $(OFILES)
	@rm -f $@
	ar rvc $@ $(OFILES)

libghttpd.so: $(OFILES)
	@rm -f $@
	$(CC) -shared -o $@ $(OFILES) -I/home/guoliqiang/git-coding/coding/third_part/gperftools/include/ -L/usr/local/lib -ldl -lpthread -L/home/guoliqiang/git-coding/coding/third_part/gperftools/lib/ -ltcmalloc -lprofiler

libghttpd-preload.so: $(OFILES) autostart.o
	@rm -f $@
	$(CC) -shared -o $@ $(OFILES) autostart.o -I/home/guoliqiang/git-coding/coding/third_part/gperftools/include/ -L/usr/local/lib -ldl -lpthread -L/home/guoliqiang/git-coding/coding/third_part/gperftools/lib/ -ltcmalloc -lprofiler

install:	$(ALL)
	cp $(ALL) /usr/local/lib
	cp gperftools-httpd.h /usr/local/include

clean:
	rm -f $(CLEANFILES)

distclean:
	rm -f $(CLEANFILES)

thttpd.o:	config.h version.h libhttpd.h fdwatch.h timers.h 
libhttpd.o:	config.h version.h libhttpd.h timers.h 
fdwatch.o:	fdwatch.h
timers.o:	timers.h


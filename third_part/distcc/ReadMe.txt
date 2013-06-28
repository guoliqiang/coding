install processing:
   ./autogen.sh
   ./configure --prefix=..
   Modified ./Makefile
   CFLAGS = -g -O2 -MD -W -Wall -Wimplicit -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Waggregate-return -Wstrict-prototypes -Wmissi     ng-prototypes -Wnested-externs -Wmissing-declarations -Wuninitialized -pthread -Wno-unused-result
   Modified  ./include_server/c_extensions/distcc_pump_c_extensions_module.c
   #include "../../../../python/include/python2.6/Python.h"
   make
   make install


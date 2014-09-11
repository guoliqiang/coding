OBJECTS = mail-file.o
LIBESMTP =-pthread -L../../lib -lesmtp -L../../../openssl/lib/ -lssl -lcrypto -L../../../libtool/lib -lltdl -ldl
CFLAGS := $(CFLAGS) -std=c99 -pedantic -O2 -g -W -Wall -I../../include -I../../../openssl/include

all: mail-file-a

mail-file-a: $(OBJECTS)
	$(CC) -g -static $(OBJECTS) $(LIBESMTP) -o mail-file-a

mail-file-so: $(OBJECTS)
	$(CC) -g $(OBJECTS) $(LIBESMTP) -o mail-file-so

clean:
	rm -f *.o core mail-file-a mail-file-so

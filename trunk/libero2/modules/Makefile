CC = gcc
INCLUDE = -I../include/libipq/ \
					-I./ \
					-I./lib/\
					-I/root/iptables-1.4.1.1/include/libipq/ \
					-I/usr/include/glib-2.0/
CFLAGS = -fpic -shared $(INCLUDE)

DATALIB =
OBJ = 
all: $(OBJ)
	$(CC) $(CFLAGS) test.c -o mod_test.so.1 -lpcre ../prebuilt/libipq.o
	chcon -t texrel_shlib_t mod_test.so.1
clean:
	rm mod_test.so.1

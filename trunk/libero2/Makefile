CC = gcc
INCLUDE = -I../include/libipq/ \
					-I./ \
					-I./lib/\
					-I/root/iptables-1.4.1.1/include/libipq/ \
					-I/usr/include/glib-2.0/
CFLAGS = $(INCLUDE) -ldl -lpcre

DATALIB = lib/glist.o lib/gqueue.o 
OBJ = prebuilt/libipq.o dump.o \
			module.o $(DATALIB)


all: $(OBJ)
	$(CC) -o a.out libero.c  $(OBJ) $(INCLUDE) -rdynamic -ldl
clean:
	rm *.o
	rm a.out

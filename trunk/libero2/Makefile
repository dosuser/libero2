CC = gcc
INCLUDE = -I./ \
					-I./lib/\
					-I/root/iptables-1.4.1.1/include/libipq/

CFLAGS = $(INCLUDE) -ldl -lpcre -lpthread
DATALIB = lib/glist.o lib/gqueue.o 
OBJ = prebuilt/libipq.o dump.o \
			module.o $(DATALIB)
all: $(OBJ)
	$(CC) -o a.out libero.c  $(OBJ) $(CFLAGS)  -rdynamic -ldl
clean:
	rm *.o
	rm a.out

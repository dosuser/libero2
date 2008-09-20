CC = gcc
INCLUDE = -I../../include/libipq/ \
					-I./ \
					-I./lib/\
					-I/root/iptables-1.4.1.1/include/libipq/ \
					-I/usr/include/glib-2.0/
CFLAGS = $(INCLUDE) -ldl -lpcre -lpthread
OBJS = ../prebuilt/libipq.o

all: $(OBJ)
	$(CC) -o a.out test.c $(CFLAGS) $(OBJS) 
2: $(OBJ)
	$(CC) -o a.out ipq.c $(CFLAGS) $(OBJS) 
	
clean:
	rm *.o
	rm a.out

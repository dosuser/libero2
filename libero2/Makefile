PWD = ~/
SITE = https://libero2.googlecode.com/svn/trunk/ --username dosuser00
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
ci:
	source ~/libero2/svn_m_read.sh
co:
	svn co $(SITE) $(PWD)

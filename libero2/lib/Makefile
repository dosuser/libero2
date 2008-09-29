CC = gcc
INCLUDE = -I./
CFLAGS = -c $(INCLUDE)
OBJS = glist.o gqueue.o logging.c
all: $(OBJS)
	$(CC) testcase.c $(OBJS) $(INCLUDE) -o testcase
clean:
	rm $(OJB) testcase

CC = gcc
CFLAGS = -g -w -O0

all:: clean my_pthread

my_pthread:
	$(CC) $(CFLAGS) my_pthread.h my_pthread.c Queue.c Queue.h myallocate.c myallocate.h parallelCal.c -o my_pthread
clean:
	rm -rf my_pthread

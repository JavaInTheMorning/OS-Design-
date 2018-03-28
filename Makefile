CC = gcc
CFLAGS = -g -w -O0

all:: clean my_pthread

my_pthread:
	$(CC) $(CFLAGS) my_pthread.c my_pthread.h Queue.c Queue.h frame.c frame.h page.c page.h myallocate.c myallocate.h shared.h parallelCalc.c -o my_pthread
clean:
	rm -rf my_pthread

CCFLAGS = -ggdb -g -pthread 
all: my_pthread

my_pthread: my_pthread.c
	gcc $(CCFLAGS) my_pthread.c my_pthread_t.h Queue.c Queue.h -o my_pthread

clean:
	rm -rf my_pthread
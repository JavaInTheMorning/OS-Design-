#ifndef MY_PTHREAD_T_H
#define MY_PTHREAD_T_H


#define _XOPEN_SOURCE 700

#include <ucontext.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>

#include <string.h>

#include "Queue.h"
#include "myallocate.h"




void prepareScheduler(long period);

void interruptHandler(int sig);

/* create a new thread */
int my_pthread_create(my_pthread_t *thread, pthread_attr_t *attr, void *(*funcAddr)(void *), void *arg);

/* give CPU pocession to other user level threads voluntarily */
int my_pthread_yield();

/* terminate a thread */
void my_pthread_exit(void *returnPointer);

/* wait for thread termination */
int my_pthread_join(my_pthread_t thread, void **value_ptr);

/* initial the mutex lock */
int my_pthread_mutex_init(my_pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);

/* aquire the mutex lock */
int my_pthread_mutex_lock(my_pthread_mutex_t *mutex);

/* release the mutex lock */
int my_pthread_mutex_unlock(my_pthread_mutex_t *mutex);

/* destroy the mutex */
int my_pthread_mutex_destroy(my_pthread_mutex_t *mutex);

void reapRoutine(void *(*start_routine)(void *), void *args);

my_pthread_t my_pthread_self();

tcb *getThreadBlockForID(my_pthread_t);

int updateNextBlock(tcb* previous);

#endif // MY_PTHREAD_T_H

// File:	my_pthread_t.h
// Author:	Yujie REN
// Date:	09/23/2017

// name:
// username of iLab:
// iLab Server: 
#ifndef MY_PTHREAD_T_H
#define MY_PTHREAD_T_H

#define _GNU_SOURCE

/* include lib header files that you need here: */
#include <unistd.h>
#include <sys/types.h>
#include <ucontext.h>
#include <pthread.h>

#include "Queue.h"

// For benchmark
#define USE_MY_PTHREAD 1 //(comment it if you want to use real pthread)

#ifdef USE_MY_PTHREAD
#define pthread_t my_pthread_t
#define pthread_mutex_t my_pthread_mutex_t
#define pthread_create my_pthread_create
#define pthread_exit my_pthread_exit
#define pthread_join my_pthread_join
#define pthread_mutex_init my_pthread_mutex_init
#define pthread_mutex_lock my_pthread_mutex_lock
#define pthread_mutex_unlock my_pthread_mutex_unlock
#define pthread_mutex_destroy my_pthread_mutex_destroy
#endif



/* mutex struct definition */
typedef struct my_pthread_mutex_t {
    void *ptr ; //a pointer to mux lock address
    const int intial ; //intialization number for a lock
    //Queue *waiting; //queue for all waiting locks 
} my_pthread_mutex_t;

/**
 * Enum containing all three priority levels.
 */
typedef enum {
    DEFAULT_PRIORITY, HIGH_PRIORITY, REALTIME_PRIORITY
} Priority;

typedef enum {
    THREAD_INITIALZED, THREAD_RUNNING, THREAD_COMPLETE
} ThreadState;

typedef uint my_pthread_t;

typedef struct threadControlBlock {
    my_pthread_t pthread_id;
    ucontext_t *ucp;

    void *stack;
    void *returnCode;

    ThreadState state;
} tcb;

/**
 * Number of priority levels.
 */
#define NUM_PRIORITY 3

/**
 * The uniform stack size UnsignedShort.MAX_SIZE
 */
#define STACK_SIZE 0x3FFF

/**
 * The default Quantum.
 */
#define DEFAULT_QUANTUM 50

/**
 * The active priority queues.
 */
Queue ACTIVE_QUEUES[NUM_PRIORITY];

/**
 * Scheduler's context, used when thread terminates or quantum is up.
 */
ucontext_t *scheduler_context = NULL;

/**
 * The scheduler's write thread identifier.
 */
pthread_t *schedule_write_pthread = NULL;

/**
 * The scheduler's read thread identifier;
 */
pthread_t *schedule_read_pthread = NULL;

/**
 * The recieved thread to schedule.
 */
QueueNode *recievedThread = NULL;

/**
 * The amount of time remaining until the next schedule.
 */
size_t nextThreadCycle = 0;

/**
 * Counter for the next available pthread identifer.
 */
my_pthread_t pthread_counter;

/**
 * Responsible for the scheduling of all @my_pthread_t.
 */
void schedule(void *);

/**
 * Responsible for execution of the current @QueueNode.
 */
void receive(void *);

/**
 * Calculates the number of both active and sleeping threads.
 * @return The number of total threads.
 */
int getNumAllThreads();

QueueNode *findForID(my_pthread_t);

/* create a new thread */
int my_pthread_create(my_pthread_t *thread, pthread_attr_t *attr, void *(*function)(void *), void *arg);

/* give CPU pocession to other user level threads voluntarily */
int my_pthread_yield();

/* terminate a thread */
void my_pthread_exit(void *value_ptr);

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

#endif

// File:	my_pthread.c
// Author:	Yujie REN
// Date:	09/23/2017

// name:
// username of iLab:
// iLab Server:


#include <errno.h>
#include <semaphore.h>
#include "my_pthread_t.h"
#include "Queue.h"

int testNumber = 0;

void *test(void *lol) {
    ++testNumber;
    printf("TEST");
}

int main(void) {
    my_pthread_t threads[2];
    int index = 0;

    for (; index < 2; index++) {
        my_pthread_create(&threads[index], NULL, (void *(*)(void *)) test, NULL);
    }

    return 0;
}


void schedule(void *argument) {
    if (pthread_detach(*schedule_write_pthread) == -1) {// Detach thread from thread group.
        perror("Could not detach thread.");
        return;
    }

    while (1) {
        if (!scheduler_context) {
            continue;
        }

        int numThreads = getNumAllThreads();
        if (numThreads == 0) {
            continue;
        }

        int priority = REALTIME_PRIORITY;
        for (; priority >= DEFAULT_PRIORITY; priority--) {
            Queue *queue = ACTIVE_QUEUES + priority;

            int numProcessed = queue->numNodes;
            do {
                QueueNode *nextThread = queue_pop(queue);
                if (!nextThread) {
                    continue;
                }

                recievedThread = nextThread;
                ((tcb *) recievedThread->data)->state = THREAD_RUNNING;
                nextThreadCycle = DEFAULT_QUANTUM << priority;

                while (1) {
                    tcb *block = (tcb *) recievedThread->data;
                    if (block->state == THREAD_COMPLETE) {
                        free(recievedThread);
                        break;
                    }

                    if (nextThreadCycle-- > 0)
                        continue;

                    if (swapcontext(block->ucp, block->ucp->uc_link) == -1) {
                        printf("Context switch failed: %d\n", errno);
                    }
                    break;
                }
            } while (--numProcessed >= 0);
        }
    }
}

void receive(void *argument) {
    if (pthread_detach(*schedule_read_pthread) == -1) {// Detach thread from thread group.
        perror("Could not detach thread.");
        return;
    }

    if (!scheduler_context) {
        scheduler_context = (ucontext_t *) malloc(sizeof(ucontext_t));
        if (!scheduler_context) {
            perror("Failed to initialize scheduler context.");
            return;
        }

        if (getcontext(scheduler_context) == -1) { // Grab this thread's ucp as it handles all processing.
            perror("Could not grab scheduler context");
            return;
        }
    }

    while (1) {
        if (!recievedThread) {
            continue;
        }

        tcb *block = (tcb *) recievedThread->data;
        if (!block) {
            continue;
        }

        if (swapcontext(scheduler_context, block->ucp) == -1) {
            printf("Context switch failed: %d\n", errno);
            continue;
        }

        block->state = THREAD_COMPLETE;
    }
}

/* create a new thread */
int my_pthread_create(my_pthread_t *thread, pthread_attr_t *attr, void *(*function)(void *), void *arg) {
    if (!schedule_write_pthread && !schedule_read_pthread) { // Lazy load the scheduler thread.
        schedule_write_pthread = (pthread_t *) malloc(sizeof(pthread_t));
        schedule_read_pthread = (pthread_t *) malloc(sizeof(pthread_t));
        if (!schedule_write_pthread || !schedule_read_pthread) {
            perror("Could not allocate scheduler thread.");
            return 0;
        }

        pthread_create(schedule_write_pthread, NULL, (void *(*)(void *)) schedule, NULL);
        pthread_create(schedule_read_pthread, NULL, (void *(*)(void *)) receive, NULL);
    }

    tcb *block = (tcb *) malloc(sizeof(tcb));
    if (!block) {
        perror("Could not allocate thread control block.");
        return -1;
    }

    block->ucp = (ucontext_t *) malloc(sizeof(ucontext_t));
    if (!block->ucp) {
        free(block);

        perror("Could not allocate user ucp protocol.");
        return -1;
    }

    getcontext(block->ucp);

    __atomic_fetch_add(&pthread_counter, 1, __ATOMIC_SEQ_CST);

    *thread = block->pthread_id = pthread_counter;

    block->ucp->uc_stack.ss_size = STACK_SIZE;
    block->ucp->uc_stack.ss_sp = block->stack = malloc(sizeof(char) * STACK_SIZE);
    if (!block->stack) {
        free(block->ucp);
        free(block);

        perror("Could not allocate stack.");
        return -1;
    }

    block->ucp->uc_link = (ucontext_t *) malloc(sizeof(ucontext_t));
    *block->ucp->uc_link = *scheduler_context;
    if (!block->ucp->uc_link) {
        perror("Could not allocate uc-link.");
        return -1;
    }
    makecontext(block->ucp, (void (*)(void)) function, 1, arg);

    block->state = THREAD_INITIALZED;
    if (!queue_push(ACTIVE_QUEUES + DEFAULT_PRIORITY, block)) {
        free(block->stack);
        free(block->ucp);
        free(block);

        perror("Could not push block onto active.");
        return -1;
    }

    return *thread;
};

/* give CPU procession to other user level threads voluntarily */
int my_pthread_yield() {
    if (!recievedThread) {
        return -1;
    }

    tcb *block = (tcb *) recievedThread->data;
    if (!queue_push(ACTIVE_QUEUES + DEFAULT_PRIORITY, block)) {
        perror("Could not push block onto active.");
        return -1;
    }
    return 0;
};

/* terminate a thread */
void my_pthread_exit(void *value_ptr) {
    if (!recievedThread) {
        return;
    }

    tcb *block = (tcb *) recievedThread->data;

    block->state = THREAD_COMPLETE;
    block->returnCode = value_ptr;
};

/* wait for thread termination */
int my_pthread_join(my_pthread_t thread, void **value_ptr) {
    tcb *block = (tcb *) recievedThread->data;
    if (thread < 0 || block->pthread_id == thread) {
        while (block->state != THREAD_COMPLETE) {
            // Spin
        }

        block->returnCode = *value_ptr;
    } else {
        QueueNode *node = findForID(thread);
        if (!node) {
            return -1;
        }

        while (((tcb *) node->data)->pthread_id != ((tcb *) recievedThread->data)->pthread_id) {
            my_pthread_yield();
        }

        return my_pthread_join(thread, value_ptr);
    }
    return 0;
};

/* initial the mutex lock */
int my_pthread_mutex_init(my_pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr) {
    return 0;
};

/* aquire the mutex lock */
int my_pthread_mutex_lock(my_pthread_mutex_t *mutex) {
    return 0;
};

/* release the mutex lock */
int my_pthread_mutex_unlock(my_pthread_mutex_t *mutex) {
    return 0;
};

/* destroy the mutex */
int my_pthread_mutex_destroy(my_pthread_mutex_t *mutex) {
    return 0;
};


int getNumAllThreads() {
    int numThreads = 0, index = REALTIME_PRIORITY;
    for (; index >= 0; index--) {
        numThreads += ACTIVE_QUEUES[index].numNodes;
    }
    return numThreads;
}

QueueNode *findForID(my_pthread_t thread) {
    QueueNode *pointer;

    int priority = REALTIME_PRIORITY;
    for (; priority >= DEFAULT_PRIORITY; priority--) {
        Queue *queue = ACTIVE_QUEUES + priority;
        if (queue->numNodes == 0) {
            continue;
        }

        pointer = queue->head;
        do {
            tcb *block = (tcb *) pointer->data;
            if (block->pthread_id == thread) {
                return pointer;
            }

            pointer = pointer->next;
        } while (pointer != queue->head);
    }

    return NULL;
}


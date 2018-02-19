// File:	my_pthread.c
// Author:	Yujie REN
// Date:	09/23/2017

// name:
// username of iLab:
// iLab Server:


#include <errno.h>
#include "my_pthread_t.h"

int testNumber = 0;

void *test(void *lol) {
    testNumber++;
    printf("Hi testing\n");
}

int main(void) {
    my_pthread_t threads[2];
    int index = 0;

    for (; index < 2; index++) {
        my_pthread_create(&threads[index], NULL, (void *(*)(void *)) test, NULL);
    }

    printf("HELLO WORLD: %d\n", testNumber);

    return 0;
}


void schedule(void *argument) {

    if (pthread_detach(*scheduler_pthread) == -1) {// Detach thread from thread group.
        perror("Could not detach thread.");
        return;
    }

    if (getcontext(&scheduler_context) == -1) { // Grab this thread's ucp as it handles all processing.
        perror("Could not grab scheduler context");
        return;
    }

    while (1) {
        int numThreads = getNumAllThreads();
        if (numThreads == 0) {
            continue;
        }

        int index = REALTIME_PRIORITY;
        for (; index >= DEFAULT_PRIORITY; index--) {
            Queue *queue = ACTIVE_QUEUES + index;

            int numProcessed = queue->numNodes;
            do {
                QueueNode *node = queue_pop(queue);
                if (!node) {
                    continue;
                }

                tcb* block = (tcb*) node->data;
                if (!block) {
                    continue;
                }

                if (swapcontext(&scheduler_context, block->ucp) == -1) {
                    printf("Context switch failed: %d\n", errno);
                    continue;
                }

                free(node);
            } while (--numProcessed >= 0);
        }
    }
}

/* create a new thread */
int my_pthread_create(my_pthread_t *thread, pthread_attr_t *attr, void *(*function)(void *), void *arg) {
    if (!scheduler_pthread) { // Lazy load the scheduler thread.
        scheduler_pthread = malloc(sizeof(pthread_t));
        if (!scheduler_pthread) {
            perror("Could not allocate scheduler thread.");
            return 0;
        }
        pthread_create(scheduler_pthread, NULL, (void *(*)(void *)) schedule, NULL);
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

    block->ucp->uc_link = &scheduler_context;
    makecontext(block->ucp, (void (*)(void)) function, 1, arg);

    if (!queue_push(ACTIVE_QUEUES + DEFAULT_PRIORITY, block)) {
        free(block->stack);
        free(block->ucp);
        free(block);

        perror("Could not push block onto active.");
        return -1;
    }

    return *thread;
};

/* give CPU pocession to other user level threads voluntarily */
int my_pthread_yield() {
    return 0;
};

/* terminate a thread */
void my_pthread_exit(void *value_ptr) {

};

/* wait for thread termination */
int my_pthread_join(my_pthread_t thread, void **value_ptr) {
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
    for (; index >= DEFAULT_PRIORITY; index--) {
        numThreads += ACTIVE_QUEUES[index].numNodes;
    }
    return numThreads;
}


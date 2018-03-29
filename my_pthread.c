#include "my_pthread.h"
#include <stdio.h>

void prepareScheduler(long period) {
    READY_QUEUE = create_queue(DEFAULT_PRIORITY);
    if (!READY_QUEUE) {
        return;
    }
    REAP_QUEUE = create_queue(DEFAULT_PRIORITY);
    if (!REAP_QUEUE) {
        return;
    }

    my_pthread_t mainThread;
    my_pthread_create(&mainThread, NULL, NULL, NULL);

    nextBlock = getThreadBlockForID(mainThread);
    if (!nextBlock) {
        printf("Could not get the main thread block. %d\n", mainThread);
        return;
    }

    it_values.it_interval.tv_usec = period;
    it_values.it_interval.tv_sec = 0;
    it_values.it_value.tv_usec = period;
    it_values.it_value.tv_sec = 0;

    sigemptyset(&alarm_sigset);
    sigaddset(&alarm_sigset, SIGVTALRM);

    if (setitimer(ITIMER_VIRTUAL, &it_values, NULL) == -1) {
        printf("Timer could not be initialized.\n");
        return;
    }

    action.sa_handler = &interruptHandler;
    if (sigaction(SIGVTALRM, &action, NULL) == -1) {
        printf("Signal could not be initialized.\n");
        return;
    }
}

int my_pthread_create(my_pthread_t *thread, pthread_attr_t *attr, void *(*funcAddr)(void *), void *arg) {
    sigprocmask(SIG_BLOCK, &alarm_sigset, NULL);

    if (!READY_QUEUE && !REAP_QUEUE) {
        prepareScheduler(5000);
    }

    tcb *block = (tcb *) malloc(sizeof(tcb));
    if (!block) {
        perror("Could not allocate thread control block.");
        return -1;
    }

    block->ucp = (ucontext_t *) malloc(sizeof(ucontext_t));
    if (!block->ucp) {
        free(block);

        perror("Could not allocate user ucp protocol.\n");
        return -1;
    }

    if (funcAddr) {
        if (getcontext(block->ucp) == -1) {
            perror("Could not get active context.\n");
            return -1;
        }
    }

    block->ucp->uc_stack.ss_sp = malloc(STACK_SIZE);
    block->ucp->uc_stack.ss_size = STACK_SIZE;
    block->ucp->uc_stack.ss_flags = 0;

    if (funcAddr) {
        block->ucp->uc_link = getThreadBlockForID(MAIN_THREAD_IDENTIFIER)->ucp;
        makecontext(block->ucp, (void (*)(void)) reapRoutine, NUM_CONTEXT_ARGS, funcAddr, arg);
    } else {
        block->ucp->uc_link = NULL;
    }

    __atomic_fetch_add(&pthread_counter, 1, __ATOMIC_SEQ_CST);
    *thread = block->pthread_id = pthread_counter;

    block->state = STATE_RUNNING;
    block->joinedThreadId = 0;

    if (!queue_push(READY_QUEUE, (void *) block)) {
        free(block->stack);
        free(block->ucp);
        free(block);

        perror("Could not push block onto active.");
        return -1;
    }

    /* unblock the signal */
    sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);
    return *thread;
}

int my_pthread_join(my_pthread_t thread, void **status) {
    if (thread == nextBlock->pthread_id)
        return -1;

    tcb *block = getThreadBlockForID(thread);
    if (!block)
        return -1;

    if (block->joinedThreadId == nextBlock->pthread_id)
        return -1;

    nextBlock->joinedThreadId = block->pthread_id;
    while (block->state == STATE_RUNNING) {
        my_pthread_yield();
    }

    if (!status) {
        return 0;
    }

    switch (block->state) {
        case STATE_RUNNING:
            printf("Thread joined in illegal state.\n");
            exit(0);
            break;
        case STATE_CANCELED:
            *status = (void *) STATE_CANCELED;
            break;
        case STATE_COMPLETE:
            *status = block->returnPointer;
            break;
    }
    return 0;
}

void my_pthread_exit(void *returnPointer) {
    sigprocmask(SIG_BLOCK, &alarm_sigset, NULL);

    if (queue_empty(READY_QUEUE)) {
        sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);
    } else if (nextBlock->pthread_id == MAIN_THREAD_IDENTIFIER) {
        do { // Perhaps we need to yield this? Possible race here?
            my_pthread_yield();
        } while (!queue_empty(READY_QUEUE));
        sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);
    } else {
        tcb *previousBlock = nextBlock;

        QueueNode *nextNode = queue_pop(READY_QUEUE);
        if (!nextNode) {
            return;
        }

        nextBlock = (tcb *) nextNode->data;

        previousBlock->state = STATE_COMPLETE;
        previousBlock->returnPointer = returnPointer;
        previousBlock->joinedThreadId = 0;

        free(previousBlock->ucp->uc_stack.ss_sp);
        free(previousBlock->ucp);

        if (!queue_push(REAP_QUEUE, previousBlock)) {
            printf("Could not push previous onto reap queue.\n");
            return;
        }

        updateProtections();
        sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);
        setcontext(nextBlock->ucp);

        nextBlock->state = STATE_COMPLETE;
        if (!queue_push(REAP_QUEUE, nextBlock)) {
            return;
        }
    }
}

int my_pthread_yield(void) {
    sigprocmask(SIG_BLOCK, &alarm_sigset, NULL);

    if (queue_empty(READY_QUEUE))
        return 0;

    tcb *previousBlock = nextBlock;

    QueueNode *nextNode = queue_pop(READY_QUEUE);
    if (!nextNode) {
        return 0;
    }

    nextBlock = (tcb *) nextNode->data;
    if (!queue_push(READY_QUEUE, previousBlock)) {
        printf("Could not push previous onto reap queue.\n");
        return -1;
    }

    updateProtections();
    sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);

    swapcontext(previousBlock->ucp, nextBlock->ucp);

    if (!queue_push(REAP_QUEUE, nextBlock)) {
        return -1;
    }

    return 0;
}

int my_pthread_mutex_init(my_pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr) {
    sigprocmask(SIG_BLOCK, &alarm_sigset, NULL);

    mutex->waiting = create_queue(DEFAULT_PRIORITY);
    if (!mutex->waiting) {
        return -1;
    }

    sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);
    return 0;
}

int my_pthread_mutex_lock(my_pthread_mutex_t *mutex) {
    sigprocmask(SIG_BLOCK, &alarm_sigset, NULL);

    if (queue_empty(mutex->waiting)) {
        if (!queue_push(mutex->waiting, (void *) my_pthread_self())) {
            printf("Could not push self into waiting queue.\n");
            return -1;
        }
        sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);
        return 0;
    }

    QueueNode *head = mutex->waiting->head;
    if (!head) {
        return -1;
    }

    if (my_pthread_self() == (my_pthread_t) head->data) {
        sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);
        return 0;
    }

    if (!queue_push(mutex->waiting, (void *) my_pthread_self())) {
        printf("Could not push self into waiting queue.\n");
        sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);
        return -1;
    }

    while (1) {
        QueueNode *nextHead = mutex->waiting->head;
        if (!nextHead)
            break;

        if (queue_empty(mutex->waiting) || my_pthread_self() == (my_pthread_t) nextHead->data)
            break;

        my_pthread_yield();
    }
    sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);
    return 0;
}

int my_pthread_mutex_unlock(my_pthread_mutex_t *mutex) {
    sigprocmask(SIG_BLOCK, &alarm_sigset, NULL);

    if (queue_empty(mutex->waiting)) {
        sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);
        return -1;
    }

    if ((my_pthread_t) mutex->waiting->head->data != my_pthread_self()) {
        sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);
        return -1;
    }

    queue_pop(mutex->waiting);
    sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);
    return 0;
}

int my_pthread_mutex_destroy(my_pthread_mutex_t *mutex) {
    sigprocmask(SIG_BLOCK, &alarm_sigset, NULL);

    //release_queue(mutex->waiting);
    sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);
    return 0;
}

void reapRoutine(void *(*start_routine)(void *), void *args) {
    sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);

    nextBlock->returnPointer = (*start_routine)(args);
    my_pthread_exit(nextBlock->returnPointer);
}

void interruptHandler(int sig) {
    sigprocmask(SIG_BLOCK, &alarm_sigset, NULL);

    tcb *previousBlock = nextBlock;
    do {
        if (queue_empty(READY_QUEUE)) {
            if (!queue_push(READY_QUEUE, previousBlock)) { //TODO higher priority push here! Block already exists.
                printf("Could not push the next block on ready interrupt.\n");
                return;
            }
            sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);
            return;
        }

        QueueNode *nextNode = queue_pop(READY_QUEUE);
        if (!nextNode) {
            continue;
        }

        nextBlock = (tcb *) nextNode->data;
        if (!nextBlock) {
            continue;
        }

        if (nextBlock->state == STATE_RUNNING) {
            break;
        }

        //my_pthread_exit(nextBlock->returnPointer);
        if (!queue_push(REAP_QUEUE, nextBlock)) {
            printf("Could not push the next block on reap interrupt.\n");
            continue;
        }

    } while (1);

    if (!queue_push(READY_QUEUE, previousBlock)) { //TODO higher priority push here! Block already exists.
        printf("Could not push the next block on ready interrupt.\n");
        return;
    }

    updateProtections();
    sigprocmask(SIG_UNBLOCK, &alarm_sigset, NULL);

    swapcontext(previousBlock->ucp, nextBlock->ucp);

    // nextBlock->state = STATE_COMPLETE;
    if (!queue_push(REAP_QUEUE, nextBlock)) {
        return;
    }
}

int updateNextBlock(tcb *lastBlock) {
    do {
        if (queue_empty(READY_QUEUE)) {
            if (lastBlock &&
                !queue_push(READY_QUEUE, lastBlock)) { //TODO higher priority push here! Block already exists.
                printf("Could not push the next block on ready interrupt.\n");
                return -1;
            }
            return -1;
        }

        QueueNode *nextNode = queue_pop(READY_QUEUE);
        if (!nextNode) {
            continue;
        }

        nextBlock = (tcb *) nextNode->data;
        if (!nextBlock) {
            continue;
        }

        if (nextBlock->state == STATE_RUNNING) {
            break;
        }

        if (!queue_push(REAP_QUEUE, nextBlock)) {
            printf("Could not push the next block on reap interrupt.\n");
            continue;
        }

    } while (1);


//    mprotect( buffer, FRAME_SIZE, PROT_NONE);  //disallow all accesses of address buffer over length pagesize
    //  mprotect( buffer, FRAME_SIZE, PROT_READ | PROT_WRITE); //allow read and write to address buffer over length pagesize

    if (!lastBlock) {
        return 0;
    }

    if (!queue_push(READY_QUEUE, lastBlock)) { //TODO higher priority push here! Block already exists.
        printf("Could not push the next block on ready interrupt.\n");
        return -1;
    }

    return 0;
}

my_pthread_t my_pthread_self() {
    return nextBlock->pthread_id;
}

tcb *getThreadBlockForID(my_pthread_t tid) {
    QueueNode *nextBlock = READY_QUEUE->head;
    do {
        tcb *t = (tcb *) nextBlock->data;
        if (t->pthread_id == tid)
            return t;
        nextBlock = nextBlock->next;
    } while (nextBlock != READY_QUEUE->tail);

    nextBlock = REAP_QUEUE->head;
    do {
        tcb *t = (tcb *) nextBlock->data;
        if (t->pthread_id == tid)
            return t;

        nextBlock = nextBlock->next;
    } while (nextBlock != REAP_QUEUE->tail);
    return NULL;
}

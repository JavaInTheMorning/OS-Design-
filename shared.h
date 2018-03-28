//
// Created by Daniel Finlay on 3/28/18.
//

/**
 * The header file for all global data.
 */

#ifndef USER_THREADS_SHARED_H
#define USER_THREADS_SHARED_H

#include <unistd.h>
#include <sys/ucontext.h>
#include <sys/time.h>

#include "Queue.h"

/*
 * General include statements??
 */
#define _XOPEN_SOURCE 700

#define FREE 1
#define NOT_FREE 0

/**
 * Main memory's size is 8MB
 */
#define BLOCK_SIZE 8000000

/**
 * The system's page size.
 */
#define PAGE_SIZE 4096
/**
 * The number of page frames.
 */
#define NUM_FRAMES (BLOCK_SIZE / PAGE_SIZE)

/**
 * The size of each chunk within a page or frame.
 */
#define CHUNK_SIZE ((PAGE_SIZE / sizeof(block)) + 1)

/**
 * The number of offset bits.
 */
#define NUM_OFFSET_BITS log2(PAGE_SIZE)

/**
 * The number of address bits.
 */
#define ADDRESS_SHIFT ((sizeof(long) * 8) - NUM_OFFSET_BITS)


typedef unsigned long my_pthread_t;

typedef enum ThreadState {
    STATE_RUNNING, STATE_CANCELED, STATE_COMPLETE
} ThreadState;

typedef enum {
    DEFAULT_PRIORITY, HIGH_PRIORITY, REALTIME_PRIORITY
} ThreadPriority;

typedef struct ThreadControlBlock {
    my_pthread_t pthread_id, joinedThreadId;
    ucontext_t *ucp;

    void *returnPointer;
    void *stack;

    ThreadState state;
} tcb;

typedef struct {
    Queue *waiting; //queue for all waiting locks
} my_pthread_mutex_t;

// Struct for story allocated memory blocks,
typedef struct {
    int free; //is block free or already allocated, if free set 1 otherwise 0
    size_t size; //size of given block allocated
    struct block *next; //pointer to next block's metadata
    struct block *prev; // pointer to prev block's metadata
} block;

typedef enum {
    TYPE_USER, TYPE_THREAD
} RequestType;

typedef enum {
    NONE = 0x0, READ = 0x1, WRITE = 0x2, READ_WRITE = 0x7
} ProtectionType;

typedef struct {
    long address, offset;
} AddressMeta;

typedef struct {
    /**
     * The physical number of the frame.
     */
    int physicalNumber;

    /**
     * The address and offset for page.
     */
    AddressMeta pageAddrMeta;

    /**
     * The boundaries of the memory array block.
     */
    long lowerBound, upperBound; // inclusive, exclusive

    /**
     * The current thread accessing the frame.
     * NOTE: Thread id 0 is for shared memory frame.
     */
    my_pthread_t threadId;

    /**
     * The chunks total size.
     */
    block *blockRoot;

    /**
     * The list of blocks within the frame.
     */
    long blockList[CHUNK_SIZE];
} Frame;

typedef struct {

    /**
     * Mask storing the attributes of the page.
     */
    long attributes;

    /**
     * The boundaries of the assigned memory.
     */
    int lowerBound, upperBound;

    /**
     * The address and offset for frame.
     */
    AddressMeta frameAddrMeta;

    // List of memory chunks that belong to the page
    Queue *pageBlockList;
} Page;

typedef struct {
    // Store curr
    tcb currentBlock;
    Page *pageList;
} pageTable;



/**
 * The protection bit range.
 */
#define PROTECTION_BIT 2
/**
 * The present bit range.
 */
#define PRESENT_BIT 3
/**
 * The valid bit range.
 */
#define VALID_BIT 4

/**
 * The reference bit range.
 */
#define REFERENCE_BIT 5

/**
 * The number of max threads.
 */
#define MAX_NUM_THREADS 0xFF

/**
 * The main thread's pthread_t.
 */
#define MAIN_THREAD_IDENTIFIER 1

/**
 * The size of the stack.
 */
#define STACK_SIZE 0xFFFF

/**
 * Number of arguments within our context creation.
 */
#define NUM_CONTEXT_ARGS 2

/**
 * If the allocator has initialized yet.
 */
static char initialized;

/**
 * Initialize memory block for simulating main memory
 */
static char memory[BLOCK_SIZE];

/**
 * The queue containing all of the ready list.
 */
static Queue *READY_QUEUE;

/**
 * The queue containing all of the reaper queue.
 */
static Queue *REAP_QUEUE;

/**
 * The next block to execute.
 */
static tcb *nextBlock;

/**
 * Contains the system timer's values.
 */
static struct itimerval it_values;

/**
 * The signal's handler and mask set.
 */
static struct sigaction action;

/**
 * The alarm's signal set, used to block unprepared context switch.
 */
static sigset_t alarm_sigset;

/**
 * The next available thread.
 */
static my_pthread_t pthread_counter;

/**
 * List of all the page tables in the program.
 * TODO possibly only store one at a time and then page swap.
 */
static pageTable pageDir[MAX_NUM_THREADS];

/**
 * List of all the frames in the program.
 */
static Frame frameList[NUM_FRAMES];

/**
 * Returns the active frame.
 * @return Active frame.
 */
Frame *getActiveFrame();

#endif //USER_THREADS_SHARED_H



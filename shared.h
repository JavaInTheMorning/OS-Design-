//
// Created by Daniel Finlay on 3/28/18.
//

/*
 * header file for all .h global variables for easier access
 */

#ifndef USER_THREADS_SHARED_H
#define USER_THREADS_SHARED_H

/*
 * General include statements??
 */
#define _XOPEN_SOURCE 700


//GLOBALS:

/*
 * Myallocate.h globals**************************************************
 */
#define FREE 1
#define NOT_FREE 0

// Macros for using our implementation
#define malloc(x) myAllocate( x , __FILE__ , __LINE__ )
#define free(x) myDeAllocate( x , __FILE__ , __LINE__ )

// Declare main memory to be 8KB
#define BLOCK_SIZE 8000000


/**
 * The size of each memory chunk.
 */
#define sizePerBlock (BLOCK_SIZE / sizeof(block) + 1)

/**
 * The number of max threads.
 */
#define MAX_NUM_THREADS 0xFF

/*
 * static pointer to point to the root of memory
 */
static block *blockRoot;

/*
 * Initialize memory block for simulating main memory
 */
static char memory[BLOCK_SIZE];

/*
 * Create an array to store pointers to memory chunk blocks in main memory
 * Initialize every index to 0, to make searching for a free block easy in findFreeSpace()
 */
static void *blockList[BLOCK_SIZE / sizeof(block) + 1] = {0};

/**
 * List of all the page tables in the program.
 * TODO possibly only store one at a time and then page swap.
 */
static pageTable pageDir[MAX_NUM_THREADS];

/**
 * List of all the frames in the program.
 */
static Frame frameList[MAX_NUM_THREADS];

// Struct for story allocated memory blocks,
typedef struct {
    int free; //is block free or already allocated, if free set 1 otherwise 0
    size_t size; //size of given block allocated
    struct block *next; //pointer to next block's metadata
    struct block *prev; // pointer to prev block's metadata
} block;


// enum for determining if its a USER or SYSTEM thread calling the malloc/free
typedef enum {
    TYPE_USER, TYPE_THREAD
} RequestType;

/*
 * GLOBALS OF frame.h***************
 */

/*
 * Number of frames in memory
 */
#define NUM_FRAMES (BLOCK_SIZE / PAGE_SIZE)


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
} Frame;


/*
 * Globals for Page.h
 */

#define PROTECTION_BIT 2
#define PRESENT_BIT 3
#define VALID_BIT 4
#define REFERENCE_BIT 5

// Declare main memory to be 8KB
#define BLOCK_SIZE 8000000

#define PAGE_SIZE sysconf(_SC_PAGE_SIZE)

#define NUM_OFFSET_BITS log2(PAGE_SIZE)
#define ADDRESS_SHIFT ((sizeof(long) * 8) - NUM_OFFSET_BITS)

typedef enum {
    NONE = 0x0, READ = 0x1, WRITE = 0x2, READ_WRITE = 0x7
} ProtectionType;

typedef struct {
    long address, offset;
} AddressMeta;

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


/*
 * Globals for my_pthread.h
 */

#define MAIN_THREAD_IDENTIFIER 1

#define STACK_SIZE 0xFFFF

#define NUM_CONTEXT_ARGS 2

static Queue *READY_QUEUE;
static Queue *REAP_QUEUE;

static tcb *nextBlock;

static struct itimerval it_values;
static struct sigaction action;
static sigset_t alarm_sigset;
static my_pthread_t pthread_counter;

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


#endif //USER_THREADS_SHARED_H



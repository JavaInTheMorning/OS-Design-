//
// Created by dholl_000 on 3/24/2018.
//

#ifndef ASST1_MYALLOCATE_H
#define ASST1_MYALLOCATE_H


#define _XOPEN_SOURCE 700

#include <unistd.h>
#include <sys/types.h>
#include <ucontext.h>
#include <pthread.h>
#include<stddef.h>
#include<stdio.h>
#include "page.h"
#include "frame.h"

#define FREE 1
#define NOT_FREE 0

// Macros for using our implementation
#define malloc(x) myAllocate( x , __FILE__ , __LINE__ )
#define free(x) myDeAllocate( x , __FILE__ , __LINE__ )

// Declare main memory to be 8KB
#define BLOCK_SIZE 8000000

#define PAGESIZE sysconf(_SC_PAGE_SIZE)

#define NUM_OFFSET_BITS log2(PAGESIZE)
#define ADDRESS_SHIFT ((sizeof(long) * 4) - NUM_OFFSET_BITS)

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

/**
 * The size of each memory chunk.
 */
#define sizePerBlock (BLOCK_SIZE / sizeof(block) + 1)

/**
 * The number of max threads.
 */
#define MAX_NUM_THREADS 0xFF

// static pointer to point to the root of memory
static block *blockRoot;

// Initialize memory block for simulating main memory
static char memory[BLOCK_SIZE];
// Create an array to store pointers to memory chunk blocks in main memory
//Initialize every index to 0, to make searching for a free block easy in findFreeSpace()
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

// Method for initalizing
void initialize();

//Get index of a free chunk(First fit)
static int findFreeSpace();

// Malloc implementation
void *myAllocate(size_t size, char *file, int line);

// Free implementation
void myDeAllocate(void *p, char *file, int line);

AddressMeta calculateAddressMeta(long addressHash);


#endif //ASST1_MYALLOCATE_H
//
// Created by dholl_000 on 3/24/2018.
//

#ifndef ASST1_MYALLOCATE_H
#define ASST1_MYALLOCATE_H
#include <unistd.h>
#include <sys/types.h>
#include <ucontext.h>
#include <pthread.h>
#include<stddef.h>
#include<stdio.h>

#define THREADREQ 1

#define malloc(x) myAllocate(x)
#define free(x) myDeAllocate(x)

#define BLOCK_SIZE 8000000

//metadata block,
typedef struct block{
    char free ; //is block free or already allocated, if free set 1 otherwise 0
    size_t size ; //size of given block allocated
    size_t pos; // the position of the given block.
    //char data[1]; //used for splitting blocks?
    struct block *next; //pointer to next block's metadata

}block;


typedef enum {
    TYPE_USER, TYPE_THREAD
}RequestType;

char memory[BLOCK_SIZE];
block *blocklist = (void*) memory;

void initialize();
void split(struct block *fitting_slot, size_t size);
void* myAllocate(size_t sizeBits);
void myDeAllocate(void *ptr);

#endif //ASST1_MYALLOCATE_H

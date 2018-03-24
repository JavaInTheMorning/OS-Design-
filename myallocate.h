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

#define malloc(x) myallocate(x, __FILE__, __LINE__, THREADREQ)
#define free(x) mydeallocate(x, __FILE__, __LINE__, THREADREQ)

void initialize();
void split(struct block *fiting_slot, size_t size) ;
void myallocate(size_t sizeBits, __FILE__, __LINE__, THREADREQ);

//metadata block,
typedef struct block{
    char free ; //is block free or already allocated, if free set 1 otherwise 0
    size_t size ; //size of given block allocated
    //char data[1]; //used for splitting blocks?
    struct block *next; //pointer to next block's metadata

};

typedef enum {
    TYPE_USER, TYPE_THREAD
}RequestType;
#endif //ASST1_MYALLOCATE_H

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
    size_t pos; // the position of the given block.
    //char data[1]; //used for splitting blocks?
    struct block *next; //pointer to next block's metadata

};
typedef struct page{
    sig_atomic_t count ; //number of references to page
    void *virtual; //kernel virtual address of page, if it mapped into kernel, null otherwise
    unsigned long flags ; //status of page, used for read/write procetions 

};

typedef enum {
    TYPE_USER, TYPE_THREAD
}RequestType;
#endif //ASST1_MYALLOCATE_H

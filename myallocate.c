//
// Created by dholl_000 on 3/24/2018.
//

#define malloc(x) myallocate(x, __FILE__, __LINE__, THREADREQ)
#define free(x) mydeallocate(x, __FILE__, __LINE__, THREADREQ)

#define BLOCK_SIZE 8000000;

#include "myallocate.h"
#include<stddef.h>
#include<stdio.h>

char memory[BLOCK_SIZE];
struct block *blocklist;

int main {
    blocklist = (void *) memory;
}

void initialize(struct block *ptr) {
    blocklist->size = BLOCK_SIZE
    -sizeof(struct block); //avaialbbe space for malloc
    blocklist->free = '1'; // initalize the block to signal it is free
    blocklist->next = NULL;
}

void
split(struct block *fill, size_t size) {//use to split memory for a requested sized that is smaller than what we have
    struct block *partiation = (void *) ((void *) fill + size + sizeof(struct block));
    partiation->size = (fill->size) - size - sizeof(struct block);
    partiation->free = '1';
    partiation->next = fill->next;
    fill->size = size;
    fill->free = '0';
    fill->next = partiation->next;

}

void* myMalloc(size_t sizeBits, RequestType) {
    struct block *current, *prev; //the current and previous memory blocks

    if (blocklist->size >= BLOCK_SIZE){
        printf("Error, not initialized");
    }
    //partiation the memory block

    if (sizeBits >= sizeof(blocklist->size)) {
        printf("error, request of memory too large");
    }

    current = blocklist; //set pointer to first block;

    //loop through to find the frist free block to use for allocation that is big enough
    while (((current->size < sizeBits) || (current->free == 0)) && (current.next != NULL)) {
        prev = current;
        current = current.next;
    }//will loop until find a block that can be allocated

    //case 1, requested size fits
    if (sizeBits >= 8) { //for large allocations
        current->position = prev->pos - sizeBits;                            /**********|*****|*****/
        current.free = '0';
        current.next = NULL;
        return (void *)(current.position);


    } else { //for small allocations
        current->position = prev->pos + prev->size; /*|******/
        current->free = '0' ;
        current.next = NULL;
        prev->next = current;//set prev next to current block that was filled
        return (void *)(current.position);
    }

}

void merge() { //to help elimatante space, merge consective free blocks of data
    struct block *current, *prev;
    current = blocklist;

    while ((current && current.next) {

        if ((current->free) && (current->next->free)) {
            current.size += (current->next->size + sizeof(struct block));
            current->next = current->next->next;
        }

        prev = current;
        current = current->next;
    }


}

void mydeallocate(void *ptr) {
    if (((void *) memory <= ptr) && (ptr <= (void *) memory + BLOCK_SIZE)){

        struct block *current = ptr;
        current = current - sizeof(current);
        current->free = 1;
        merge();
    }
    else{
        printf("unable to deallocate given pointer");
    }
}





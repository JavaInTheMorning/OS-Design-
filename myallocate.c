//
// Created by dholl_000 on 3/24/2018.
//



#include "myallocate.h"


void initialize(struct block *ptr) {
    blocklist->size = BLOCK_SIZE -sizeof(block); //avaialbbe space for malloc
    blocklist->free = '1'; // initalize the block to signal it is free
    blocklist->next = NULL;
}

void split(block *fill, size_t size) {//use to split memory for a requested sized that is smaller than what we have
    block *partition = (void *) ((void *) fill + size + sizeof(block));
    partition->size = (fill->size) - size - sizeof(block);
    partition->free = '1';
    partition->next = fill->next;
    fill->size = size;
    fill->free = '0';
    fill->next = partition->next;

}

void* myAllocate(size_t sizeBits) {
    block *current, *prev; //the current and previous memory blocks

    if (blocklist->size >= BLOCK_SIZE){
        printf("Error, not initialized");
    }
    //partiation the memory block

    if (sizeBits >= sizeof(blocklist->size)) {
        printf("error, request of memory too large");
    }

    current = blocklist; //set pointer to first block;

    //loop through to find the frist free block to use for allocation that is big enough
    while (((current->size < sizeBits) || (current->free == 0)) && (current->next != NULL)) {
        prev = current;
        current = current->next;
    }//will loop until find a block that can be allocated

    //case 1, requested size fits
    if (sizeBits >= 8) { //for large allocations
        current->pos = prev->pos - sizeBits;                            /**********|*****|*****/
        current->free = '0';
        current->next = NULL;
        return (void *)(current->pos);


    } else { //for small allocations
        current->pos = prev->pos + prev->size; /*|******/
        current->free = '0' ;
        current->next = NULL;
        prev->next = current;//set prev next to current block that was filled
        return (void *)(current->pos);
    }

}

void merge() { //to help elimatante space, merge consective free blocks of data
    block *current, *prev;
    current = blocklist;

    while (current && current->next) {

        if ((current->free) && (current->next->free)) {
            current->size += (current->next->size + sizeof(block));
            current->next = current->next->next;
        }

        prev = current;
        current = current->next;
    }


}

void myDeAllocate(void *ptr) {
    if (((void *) memory <= ptr) && (ptr <= (void *) memory + BLOCK_SIZE)){

        block *current = ptr;
        current = current - sizeof(current);
        current->free = 1;
        merge();
    }
    else{
        printf("unable to deallocate given pointer");
    }
}





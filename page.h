//
// Created by Daniel Finlay on 3/24/2018.
//

#ifndef ASST1_PAGE_H
#define ASST1_PAGE_H
#define PAGESIZE = sysconf(_SC_PAGE_SIZE)

#include <stdlib.h>

#include "Queue.h"
#include "my_pthread.h"

#define PROTECTION_BIT 1
#define PRESENT_BIT 2
#define VALID_BIT 3
#define REFERENCE_BIT 11

typedef enum {
    READ = 1, WRITE = 2, READ_WRITE = 3
} ProtectionType;

typedef struct {
    long address, offset;
} AddressMeta;

typedef struct {
    /**
     * Mask storing the attributes of the page.
     */
    long attributes;

    // frame number(physical) that the page is mapped to(virtual)
    // Has total page pointer, actual address & offset
    struct AddressMeta *pageAddress;

    // Set equal to Macro PAGESIZE on initialization
    int *pageSize;

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


// Method used to lookup Physical Frame from virtual page
struct AddressMeta getFrameFromPage(AddressMeta pageaAddress, pageTable pt);

// Used to set a page of a given pageTable to a frame
void *setPageToAFrame(AddressMeta pageAddress, pageTable pt);


#endif //OS_DESIGN_PROJECT3_PAGE_H

/*int main()
{
    // Get a pointer
    int *p;
    p = (int*) malloc(sizeof(int));
    *p = 25;
    printf("address: %p, value: %d\n", p, *p);


    // access metadata
    block *temp;
    temp = (block*)((char*)p - sizeof(block));

    if (temp->free == FREE)
        printf("IS FREE\n");
    else if (temp->free == NOT_FREE)
        printf("NOT FREE\n");

    printf("currentsize(%d):\n", temp->size);
    printf("address: %p, value: %d\n", temp, *temp);

    // Access value from metadata pointer
    int *k;
    k = (int*)((char*)temp + sizeof(block));
    printf("address: %p, value: %d\n", k, *k);





    printf("Success\n");*/
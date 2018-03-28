//
// Created by Daniel Finlay on 3/24/2018.
//

#ifndef ASST1_PAGE_H
#define ASST1_PAGE_H

#include <stdlib.h>
#include <unistd.h>

#include "Queue.h"
#include "my_pthread.h"

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


// Method used to lookup Physical Frame from virtual page
struct AddressMeta getFrameFromPage(AddressMeta pageaAddress, pageTable pt);

// Used to set a page of a given pageTable to a frame
void *setPageToAFrame(AddressMeta pageAddress, pageTable pt);

ProtectionType getProtectionType(Page *);

void setProtectionType(Page *, ProtectionType);

long getPresentBit(Page *);

void setPresentBit(Page *, int);

long getValidBit(Page *);

void setValidBit(Page *, int);

long getReferenceBit(Page *);

void setReferenceBit(Page *, int);

void updateAllAttributes(Page *, ProtectionType, long, long, long);


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
//
// Created by Daniel Finlay on 3/24/2018.
//

#ifndef ASST1_PAGE_H
#define ASST1_PAGE_H

#include "Queue.h"

typedef struct page {
    // Either 'r'(Read),'w'(Write),'e'(Execute)
    char *protectionBit;

    // Either 'm'(for in physical memory), 'f'(for in swapped file)
    char *presentBit;

    // 't'(valid) 'f'(invalid
    char *isValid;

    // Increment on every reference/Call to determine least visited pages for swapping
    int *referenceBit;

    // frame number(physical) that the page is mapped to(virtual)
    int *pageFrameNumber;

    // Set equal to Macro PAGESIZE on initialization
    int *pageSize;

    // For storing virtual address the page corresponds too.
    void *virtualAddress;

    // Offset for mapping page to corresponding frame
    int *offset;

    // List of memory chunks that belong to the page
    Queue *pageBlockList;
};


#endif //OS_DESIGN_PROJECT3_PAGE_H

//
// Created by Daniel Finlay on 3/24/2018.
//

#ifndef ASST1_PAGE_H
#define ASST1_PAGE_H
#define PAGESIZE = sysconf(_SC_PAGE_SIZE)
#include "queue.h"

typedef struct page{
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

    // List of memory chunks that belong to the page
    queue* pageBlockList;
};



#endif //OS_DESIGN_PROJECT3_PAGE_H

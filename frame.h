//
// Created by Daniel Finlay on 3/24/2018.
//

#ifndef ASST1_FRAME_H
#define ASST1_FRAME_H

#include "page.h"

#define NUM_FRAMES (BLOCK_SIZE / PAGE_SIZE)

typedef struct {
    /**
     * The physical number of the frame.
     */
    int physicalNumber;

    /**
     * The address and offset for page.
     */
    AddressMeta pageAddrMeta;

    /**
     * The boundaries of the memory array block.
     */
    long lowerBound, upperBound; // inclusive, exclusive

    /**
     * The current thread accessing the frame.
     * NOTE: Thread id 0 is for shared memory frame.
     */
    my_pthread_t threadId;
} Frame;


Frame *init();


#endif //ASST1_FRAME_H

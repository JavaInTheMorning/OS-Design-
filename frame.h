//
// Created by Daniel Finlay on 3/24/2018.
//

#ifndef ASST1_FRAME_H
#define ASST1_PROJECT3_FRAME_H


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
    int minBound, highBound;

    /**
     * The current thread accessing the frame.
     * NOTE: Thread id 0 is for shared memory frame.
     */
    my_pthread_t threadId;
} Frame;

#endif //OS_DESIGN_PROJECT3_FRAME_H

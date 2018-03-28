//
// Created by Daniel Finlay on 3/24/2018.
//

#include <memory.h>
#include "frame.h"

Frame *initFrames() {
    int physicalNumber = 0;
    for (; physicalNumber < NUM_FRAMES; physicalNumber++) {
        long lowerBound = physicalNumber * PAGE_SIZE;
        long upperBound = (physicalNumber + 1) * PAGE_SIZE;

        Frame *frame = (frameList + physicalNumber);

        frame->lowerBound = lowerBound;
        frame->upperBound = upperBound;

        frame->physicalNumber = physicalNumber;

        frame->blockRoot = (block *) (memory + lowerBound);
        // initialize prev/next to null
        frame->blockRoot->prev = 0;
        frame->blockRoot->next = 0;
        // Initialize root size to be total memory available - metadata for root
        frame->blockRoot->size = upperBound - lowerBound - sizeof(block);
        // Initialize the block to be free
        frame->blockRoot->free = FREE;

        memset(frame->blockList, 0, sizeof(frame->blockList));
    }

    return (Frame *) &frameList;
}

int findFreeSpace(Frame *frame) {
    int index;
    for (index = 0; index < CHUNK_SIZE; index++) {
        if (frame->blockList[index] == 0) { // return index in array of pointers that's free/not initialized
            return index;
        }
    }

    return -1;
}

Frame *getActiveFrame() {
    if (!nextBlock) {
        return frameList;
    }
    return frameList + nextBlock->pthread_id;
}

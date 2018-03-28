//
// Created by Daniel Finlay on 3/24/2018.
//

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

        frame->nextBlock = (block *) memory + lowerBound;
        // initialize prev/next to null
        frame->nextBlock->prev = 0;
        frame->nextBlock->next = 0;
        // Initialize root size to be total memory available - metadata for root
        frame->nextBlock->size = upperBound - lowerBound - sizeof(block);
        // Initialize the block to be free
        frame->nextBlock->free = '1';

        frame->numBlocks = (frame->nextBlock->size / sizeof(block)) + 1;

        char blockList[frame->numBlocks] = {0};
        frame->blockList = blockList;

        frame->blockList[findFreeSpace(frame)] = memory + lowerBound;

    }

    return (Frame *) &frameList;
}

int findFreeSpace(Frame *frame) {
    int index;
    for (index = 0; index < frame->numBlocks; index++) {
        if (frame->blockList[index] == 0) { // return index in array of pointers that's free/not initialized
            return index;
        }
    }

    return -1;
}

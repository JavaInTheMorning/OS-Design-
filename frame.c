//
// Created by Daniel Finlay on 3/24/2018.
//

#include "frame.h"

Frame *init() {
    static Frame frameList[50000];

    int physicalNumber = 0;
    for (; physicalNumber < NUM_FRAMES; physicalNumber++) {
        long lowerBound = physicalNumber * PAGE_SIZE;
        long upperBound = (physicalNumber + 1) * PAGE_SIZE;

        Frame* frame = &frameList[physicalNumber];

        frame->lowerBound = lowerBound;
        frame->upperBound = upperBound;

        frame->physicalNumber = physicalNumber;

    }

    return (Frame *) &frameList;
}


int main(void) {
    Frame *frames = init();


    int index = 0;
    for (; index < NUM_FRAMES; index++) {
        Frame *frame = frames + index;

        printf("Frame %d %li %li\n", frame->physicalNumber, frame->lowerBound, frame->upperBound);
    }

    return 0;
}

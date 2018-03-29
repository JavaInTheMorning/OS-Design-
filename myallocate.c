//
// Created by dholl_000 on 3/24/2018.
//

#include "myallocate.h"

#define META_SIZE 1

void initialize() {
    initFrames();
    initPages();
}

void *myAllocate(size_t sizeBits, char *fileName, int lineNum, RequestType type) {
    if (sizeBits == 0) { // You can't allocate nothing.
        return NULL;
    }

    if (!initialized) {
        initialized = 1;
        initialize();
    }

    PageTable* table = getActiveTable();


    int pageNum = 0;
    size_t position = 0;

    char isOpenAvail = 0;
    Page *page = NULL;
    for (; pageNum < NUM_FRAMES; pageNum++) {
        page = table->pages + pageNum;
        for (; position < FRAME_SIZE; position++) {
            // Need to find contiguous space in memory.
            size_t fromPosition = position, toPosition = fromPosition + sizeBits + META_SIZE;

            if (toPosition > FRAME_SIZE) {
                isOpenAvail = NULL;
                break;
            }

            isOpenAvail = 1;
            for (; fromPosition < toPosition; fromPosition++) {
                if (page->frame->blockList[fromPosition] != FREE) { // Something isn't in here we continue.
                    continue;
                }

                isOpenAvail = NULL;
                break;
            }

            if (isOpenAvail) {
                break;
            }
        }

        if (isOpenAvail) {
            break;
        }
    }

    if (!isOpenAvail) {
        return NULL;
    }


    page->frame->blockList[position] = sizeBits;

    size_t index = position + META_SIZE;
    for (; index < sizeBits + META_SIZE; index++) {
        page->frame->blockList[index] = (long) (memory + page->frame->lowerBound +
                                                index); // no real memory here only placibos
    }

    setPageValidBit(page, 0);

    return memory + page->frame->lowerBound + position + META_SIZE;
}

void myDeAllocate(void *ptr, char *fileName, int lineNum, RequestType type) {
    PageTable* table = getActiveTable();

    int pageNum = 0;
    size_t position = 0;
    char pageFound = 0;
    Page *page;

    for (; pageNum < NUM_FRAMES; pageNum++) {
        page = table->pages + pageNum;

        if (getPageValidBit(page) != 0) {
            continue;
        }

        for (; position < FRAME_SIZE; position++) { // <__ wrong code here
            void *address = ((void *) page->frame->blockList[position]);
            if (address == ptr) {
                pageFound = 1;
                break;
            }
        }

        if (pageFound) {
            break;
        }
    }


    if (!pageFound) {
        return;
    }

    size_t toPosition = position + page->frame->blockList[position - META_SIZE];
    size_t index = position - META_SIZE;
    for (; index < toPosition; index++) {
        page->frame->blockList[index] = FREE;
    }

    *(&(ptr)) = NULL;
}


AddressMeta calculateAddressMeta(long addressHash) {
    AddressMeta meta;

    meta.offset = addressHash & ((int) pow(2.0, (double) NUM_OFFSET_BITS));
    meta.address = (addressHash >> (long) ADDRESS_SHIFT);

    return meta;
}
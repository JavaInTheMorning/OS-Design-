//
// Created by dholl_000 on 3/24/2018.
//



#include "myallocate.h"
#include "frame.h"

void initialize() {
    initFrames();
}

void *myAllocate(size_t sizeBits, char *fileName, int lineNum, RequestType type) {
    Frame* frame = getActiveFrame();
    block *current, *next; //the current and next memory blocks to set the new(next) pointer to the allocated memory chunk

    // Check to make sure the user is requesting more than 0 bytes
    if (sizeBits == 0) {
        fprintf(stderr, "Error: 0 bytes requested. FILE: '%s' LINE: '%d'\n", fileName, lineNum);
        return 0;
    }

    // If the necessary resources haven't been initialized -> initialized them
    if (!initialized)
        initialize();

    // set the current block to the root for traversing through memory
    current = frame->nextBlock;
    do {
        if (current->size < sizeBits || current->free == NOT_FREE) {
            // If the size requested is larger than the current size available or it's not free-> continue
            current = current->next;
        } else if (current->size < (sizeBits + sizeof(block))) {
            // The chunk is large enough to store memory request but not big enough to store enough metadata for next request
            // Thus, can't do anymore requests in this chunk -> set to notFree
            current->free = NOT_FREE;
            //return memory request
            return (char *) current + sizeof(block);
        } else {
            // Otherwise it found a spot that's free & has enough space for the metadata/size of the next chunk

            // initialize the next chunk & it's attributes to append to the blockList for future use
            // Next location will be an offset from the location of current plus the metadata/data size
            next = (block *) ((char *) current + sizeof(block) + sizeBits);
            // set the next block to point back to the current block
            next->prev = current;
            // set the next's next block to point to what the current block is pointing at
            next->next = current->next;
            // Initialize the size left available to the current size - metadata & data
            next->size = current->size - sizeof(block) - sizeBits;
            // Now let future calls know that this chunk is now free for allocation
            next->free = FREE;
            // Get the nextfreespace to store this new pointer
            frame->blockList[findFreeSpace()] = next;

            //Initialize the current pointer to be returned to memory allocation call
            // Set size to the size requested
            current->size = sizeBits;
            // Set that it is no longer free
            current->free = NOT_FREE;
            // Set it's next to the new free block just initialized
            current->next = next;
            // Return pointer
            return (char *) current + sizeof(block);
        }
        // While current is != null
    } while (current != 0);

    // The memory allocation failed
    fprintf(stderr, "Not enough space available for memory request of size %d bytes. FILE: '%s' LINE: '%d'\n",
            sizeBits, fileName, lineNum);
}

void myDeAllocate(void *ptr, char *fileName, int lineNum, RequestType type) {
    Frame* frame = getActiveFrame();
    block *current, *next, *prev;

    if (ptr == NULL) {
        fprintf(stderr, "ERROR: Ptr is NULL. FILE: '%s' LINE: '%d'\n", fileName, lineNum);
        return;
    }
    // Set current
    current = (block *) ((char *) ptr - sizeof(block));
    int i;
    // Flag to check ptr has been malloced & not freed yet
    int isFreeable;

    for (i = 0; i < sizePerBlock; i++) {
        if (current == frame->blockList[i] && current->free == NOT_FREE) {
            // if the ptr to be freed is found in the malloced list, & it's NOT been freed
            // Then it's freeable
            isFreeable = 1;
            break;
        }
    }

    if (!isFreeable) {
        fprintf(stderr, "ERROR: Pointer has not been malloced, or has already been freed. FILE: '%s' LINE: '%d'\n",
                fileName, lineNum);
        return;
    }

    // Merge with previous chunk
    if ((prev = current->prev) != 0 && prev->free == FREE) {
        // Merge the previous chunk with this chunk since they're free
        prev->size += sizeof(block) + current->size;
        // To free it remove it from the blocklist
        frame->blockList[i] = 0;
    } else {
        current->free = FREE;
        prev = current;
    }

    // Merge with next chunk
    if ((next = current->next) != 0 && next->free == FREE) {
        prev->size += sizeof(block) + next->size;
        prev->next = next->next;
        for (i = 0; i < sizePerBlock; i++) {
            if (next == frame->blockList[i]) {
                frame->blockList[i] = 0;
                break;
            }
        }
    }
}


AddressMeta calculateAddressMeta(long addressHash) {
    AddressMeta meta;

    meta.offset = addressHash & ((int) pow(2.0, (double) NUM_OFFSET_BITS));
    meta.address = (addressHash >> (long) ADDRESS_SHIFT);

    return meta;
}
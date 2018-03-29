#include "frame.h"
#include "shared.h"

void initFrames() {
    int physicalNumber = 0;
    for (; physicalNumber < NUM_FRAMES; physicalNumber++) {
        long lowerBound = physicalNumber * FRAME_SIZE;
        long upperBound = (physicalNumber + 1) * FRAME_SIZE;

        Frame *frame = (frameList + physicalNumber);

        frame->lowerBound = lowerBound;
        frame->upperBound = upperBound;
        frame->attributes = 0;

        frame->physicalNumber = physicalNumber;
    }
}

ProtectionType getProtectionType(Frame *frame) {
    long attributes = frame->attributes;

    return (ProtectionType) attributes & 0x7;
}

void setProtectionType(Frame *frame, ProtectionType type) {
    updateAllAttributes(frame, type, getOccupiedBit(frame), getPresentBit(frame), getValidBit(frame), getReferenceBit(frame));
}

long getPresentBit(Frame *frame) {
    long attributes = frame->attributes;

    return (attributes >> PRESENT_BIT) & 0x1;
}

void setPresentBit(Frame *frame, int present) {
    frame->attributes |= present << PRESENT_BIT;

    updateAllAttributes(frame, getProtectionType(frame), getOccupiedBit(frame), present, getValidBit(frame), getReferenceBit(frame));
}

long getValidBit(Frame *frame) {
    long attributes = frame->attributes;

    return (attributes >> VALID_BIT) & 0x1;
}

void setValidBit(Frame *frame, int valid) {
    updateAllAttributes(frame, getProtectionType(frame), getOccupiedBit(frame), getPresentBit(frame), valid, getReferenceBit(frame));
}


long getOccupiedBit(Frame *frame) {
    long attributes = frame->attributes;

    return (attributes >> OCCUPIED_BIT) & 1;
}

void setOccupiedBit(Frame *frame, int occupiedBit) {
    updateAllAttributes(frame, getProtectionType(frame), occupiedBit,  getPresentBit(frame), getValidBit(frame), getReferenceBit(frame));
}

long getReferenceBit(Frame *frame) {
    long attributes = frame->attributes;

    return (attributes >> REFERENCE_BIT) & 0xFFFF;
}

void setReferenceBit(Frame *frame, int referenceBit) {
    updateAllAttributes(frame, getProtectionType(frame), getOccupiedBit(frame), getPresentBit(frame), getValidBit(frame), referenceBit);
}

void updateAllAttributes(Frame *frame, ProtectionType type, long occupiedBit, long presentBit, long validBit, long referenceBit) {
    frame->attributes = referenceBit << REFERENCE_BIT | occupiedBit << OCCUPIED_BIT | validBit << VALID_BIT | presentBit << PRESENT_BIT | type;
}

void setPage(my_pthread_t threadId, Page* page, Frame* frame) {
    setOccupiedBit(frame, 1);

    frame->threadId = threadId;
    page->frame = frame;
}
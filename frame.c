#include "frame.h"

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
        memset(frame->blockList, -1, sizeof(frame->blockList));
    }
}

ProtectionType getProtectionType(Frame *frame) {
    long attributes = frame->attributes;

    return (ProtectionType) attributes & 0x7;
}

void setProtectionType(Frame *frame, ProtectionType type) {
    updateAllAttributes(frame, type, getPresentBit(frame), getValidBit(frame), getReferenceBit(frame));
}

long getPresentBit(Frame *frame) {
    long attributes = frame->attributes;

    return (attributes >> PRESENT_BIT) & 0x1;
}

void setPresentBit(Frame *frame, int present) {
    frame->attributes |= present << PRESENT_BIT;

    updateAllAttributes(frame, getProtectionType(frame), present, getValidBit(frame), getReferenceBit(frame));
}

long getValidBit(Frame *frame) {
    long attributes = frame->attributes;

    return (attributes >> VALID_BIT) & 0x1;
}

void setValidBit(Frame *frame, int valid) {
    updateAllAttributes(frame, getProtectionType(frame), getPresentBit(frame), valid, getReferenceBit(frame));
}

long getReferenceBit(Frame *frame) {
    long attributes = frame->attributes;

    return (attributes >> REFERENCE_BIT) & 0xFFFF;
}

void setReferenceBit(Frame *frame, int referenceBit) {
    updateAllAttributes(frame, getProtectionType(frame), getPresentBit(frame), getValidBit(frame), referenceBit);
}

void updateAllAttributes(Frame *frame, ProtectionType type, long presentBit, long validBit, long referenceBit) {
    frame->attributes = referenceBit << REFERENCE_BIT | validBit << VALID_BIT | presentBit << PRESENT_BIT | type;
}
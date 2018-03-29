//
// Created by Daniel Finlay on 3/24/2018.
//

#ifndef ASST1_FRAME_H
#define ASST1_FRAME_H

#include <memory.h>
#include "shared.h"

void initFrames();

/**
 * The protection that the frame enables.
 */
ProtectionType getProtectionType(Frame *);

void setProtectionType(Frame *, ProtectionType);

/**
 * Determines whether the frame is in memory or stored in a file('0' for memory '1' for file
 */
long getPresentBit(Frame *);

void setPresentBit(Frame *, int);

/**
 *
 */
long getValidBit(Frame *);

void setValidBit(Frame *, int);

long getReferenceBit(Frame *);

void setReferenceBit(Frame *, int);

void updateAllAttributes(Frame *, ProtectionType, long, long, long);

#endif //ASST1_FRAME_H
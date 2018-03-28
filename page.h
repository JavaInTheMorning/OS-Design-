//
// Created by Daniel Finlay on 3/24/2018.
//

#ifndef ASST1_PAGE_H
#define ASST1_PAGE_H

#include "shared.h"

// Method used to lookup Physical Frame from virtual page
struct AddressMeta getFrameFromPage(AddressMeta pageaAddress, pageTable pt);

// Used to set a page of a given pageTable to a frame
void *setPageToAFrame(AddressMeta pageAddress, pageTable pt);

ProtectionType getProtectionType(Page *);

void setProtectionType(Page *, ProtectionType);

long getPresentBit(Page *);

void setPresentBit(Page *, int);

long getValidBit(Page *);

void setValidBit(Page *, int);

long getReferenceBit(Page *);

void setReferenceBit(Page *, int);

void updateAllAttributes(Page *, ProtectionType, long, long, long);


#endif //ASST1_PAGE_H
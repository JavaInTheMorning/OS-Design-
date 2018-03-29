//
// Created by Daniel Finlay on 3/24/2018.
//

#ifndef ASST1_PAGE_H
#define ASST1_PAGE_H

#include <memory.h>
#include "shared.h"

void initPages();

ProtectionType getPageProtectionBit(Page *);

void setPageProtectionBit(Page *, ProtectionType);

long getPagePresentBit(Page *);

void setPagePresentBit(Page *, int);

long getPageValidBit(Page *);

void setPageValidBit(Page *, int);

long getPageReferenceBit(Page *);

void setPageReferenceBit(Page *, int);

void updatePageAttributes(Page *, ProtectionType, long, long, long);

PageTable *getActiveTable();

#endif //ASST1_PAGE_H
//
// Created by Daniel Finlay on 3/24/2018.
//

#ifndef ASST1_PAGE_H
#
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


#endif //OS_DESIGN_PROJECT3_PAGE_H

/*int main()
{
    // Get a pointer
    int *p;
    p = (int*) malloc(sizeof(int));
    *p = 25;
    printf("address: %p, value: %d\n", p, *p);
    // access metadata
    block *temp;
    temp = (block*)((char*)p - sizeof(block));
    if (temp->free == FREE)
        printf("IS FREE\n");
    else if (temp->free == NOT_FREE)
        printf("NOT FREE\n");
    printf("currentsize(%d):\n", temp->size);
    printf("address: %p, value: %d\n", temp, *temp);
    // Access value from metadata pointer
    int *k;
    k = (int*)((char*)temp + sizeof(block));
    printf("address: %p, value: %d\n", k, *k);
    printf("Success\n");*/
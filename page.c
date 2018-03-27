//
// Created by Daniel Finlay on 3/24/2018.
//

#include "page.h"

int RANGE_MOD(int x, int L, int R) {
    return x & ~(((1UL << (L - R + 1)) - 1) << R);
}

ProtectionType getProtectionType(Page* page) {
    int attributes = page->attributes;

    return (ProtectionType) attributes & 0x3; // 0 - 3
}

void setProtectionType(Page* page, ProtectionType type) {
    page->attributes |= RANGE_MOD(type, 0, PROTECTION_BIT);
}

int getPresentBit(Page* page) {
    int attributes = page->attributes;

    return (attributes >> PRESENT_BIT) & 0x1;
}

void setPresentBit(Page* page, int present) {
    page->attributes |= present << PRESENT_BIT;
}

int getValidBit(Page* page) {
    int attributes = page->attributes;

    return (attributes >> VALID_BIT) & 0x1;
}

void setValidBit(Page* page, int valid) {
    page->attributes |= valid << VALID_BIT;// RANGE_MOD(valid, VALID_BIT, VALID_BIT);
}

int getReferenceBit(Page* page) {
    int attributes = page->attributes;

    return (attributes >> (VALID_BIT + 1)) & 0xFFFF;
}

void setReferenceBit(Page* page, int references) {
    page->attributes |= references << (VALID_BIT + 1);
}



int main(void) {

    Page page;

    //setProtectionType(&page, READ_WRITE);
    setProtectionType(&page, READ);
    setPresentBit(&page, 1); // TODO maybe constants?

    printf("ATTR: %d\n", RANGE_MOD(50, REFERENCE_BIT, VALID_BIT + 1));
    setReferenceBit(&page, 50);
    setReferenceBit(&page, 3);

    printf("%d %d %d\n", getProtectionType(&page),
           getPresentBit(&page), getReferenceBit(&page));


}

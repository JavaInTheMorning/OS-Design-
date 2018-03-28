//
// Created by Daniel Finlay on 3/24/2018.
//

#include "page.h"

ProtectionType getProtectionType(Page *page) {
    long attributes = page->attributes;

    return (ProtectionType) attributes & 0x7;
}

void setProtectionType(Page *page, ProtectionType type) {
    updateAllAttributes(page, type, getPresentBit(page), getValidBit(page), getReferenceBit(page));
}

long getPresentBit(Page *page) {
    long attributes = page->attributes;

    return (attributes >> PRESENT_BIT) & 0x1;
}

void setPresentBit(Page *page, int present) {
    page->attributes |= present << PRESENT_BIT;

    updateAllAttributes(page, getProtectionType(page), present, getValidBit(page), getReferenceBit(page));
}

long getValidBit(Page *page) {
    long attributes = page->attributes;

    return (attributes >> VALID_BIT) & 0x1;
}

void setValidBit(Page *page, int valid) {
    updateAllAttributes(page, getProtectionType(page), getPresentBit(page), valid, getReferenceBit(page));
}

long getReferenceBit(Page *page) {
    long attributes = page->attributes;

    return (attributes >> REFERENCE_BIT) & 0xFFFF;
}

void setReferenceBit(Page *page, int referenceBit) {
    updateAllAttributes(page, getProtectionType(page), getPresentBit(page), getValidBit(page), referenceBit);
}

void updateAllAttributes(Page *page, ProtectionType type, long presentBit, long validBit, long referenceBit) {
    page->attributes = referenceBit << REFERENCE_BIT | validBit << VALID_BIT | presentBit << PRESENT_BIT | type;
}
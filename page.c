#include "page.h"

void initPages() {
    int virtualNumber = 0;
    for (; virtualNumber < MAX_NUM_THREADS; virtualNumber++) {
        PageTable *table = (pageDir + virtualNumber);

        int pageNum = 0;
        for (; pageNum < NUM_FRAMES; pageNum++) {
            Page *page = table->pages + pageNum;

            page->frame = frameList + pageNum;

            setPageValidBit(page, 1);
        }
    }
}

ProtectionType getPageProtectionBit(Page *page) {
    long attributes = page->attributes;

    return (ProtectionType) attributes & 0x7;
}

void setPageProtectionBit(Page *page, ProtectionType type) {
    updatePageAttributes(page, type, getPagePresentBit(page), getPageValidBit(page), getPageReferenceBit(page));
}

long getPagePresentBit(Page *page) {
    long attributes = page->attributes;

    return (attributes >> PRESENT_BIT) & 0x1;
}

void setPagePresentBit(Page *page, int present) {
    page->attributes |= present << PRESENT_BIT;

    updatePageAttributes(page, getPageProtectionBit(page), present, getPageValidBit(page), getPageReferenceBit(page));
}

long getPageValidBit(Page *page) {
    long attributes = page->attributes;

    return (attributes >> VALID_BIT) & 0x1;
}

void setPageValidBit(Page *page, int valid) {
    updatePageAttributes(page, getPageProtectionBit(page), getPagePresentBit(page), valid, getPageReferenceBit(page));
}

long getPageReferenceBit(Page *page) {
    long attributes = page->attributes;

    return (attributes >> REFERENCE_BIT) & 0xFFFF;
}

void setPageReferenceBit(Page *page, int referenceBit) {
    updatePageAttributes(page, getPageProtectionBit(page), getPagePresentBit(page), getPageValidBit(page),
                         referenceBit);
}

void updatePageAttributes(Page *page, ProtectionType type, long presentBit, long validBit, long referenceBit) {
    page->attributes = referenceBit << REFERENCE_BIT | validBit << VALID_BIT | presentBit << PRESENT_BIT | type;
}

PageTable* getActiveTable() {
    return &pageDir[!nextBlock ? 0 : nextBlock->pthread_id];
}
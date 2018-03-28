#ifndef MY_ALLOC_H
#define MY_ALLOC_H

#include <stdio.h>
#include <strings.h>
#include <tgmath.h>

#include "shared.h"

void initialize();

static int findFreeSpace();

void *myAllocate(size_t size, char *file, int line, RequestType type);

void myDeAllocate(void *p, char *file, int line, RequestType type);

AddressMeta calculateAddressMeta(long addressHash);

#endif //MY_ALLOC_H
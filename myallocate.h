#ifndef MY_ALLOC_H
#define MY_ALLOC_H

#include <stdio.h>
#include <strings.h>
#include <tgmath.h>

#include "shared.h"

void initialize();

void *myAllocate(size_t size, char *file, int line, RequestType type);

void myDeAllocate(void *p, char *file, int line, RequestType type);

AddressMeta calculateAddressMeta(long addressHash);

// Macros for using our implementation
#define malloc(x) myAllocate( x , __FILE__ , __LINE__, TYPE_THREAD)
#define free(x) myDeAllocate( x , __FILE__ , __LINE__, TYPE_THREAD)

#endif //MY_ALLOC_H
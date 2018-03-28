//
// Created by dholl_000 on 3/24/2018.
//


// Method for initalizing
void initialize();

//Get index of a free chunk(First fit)
static int findFreeSpace();

// Malloc implementation
void *myAllocate(size_t size, char *file, int line);

// Free implementation
void myDeAllocate(void *p, char *file, int line);

AddressMeta calculateAddressMeta(long addressHash);


#endif //ASST1_MYALLOCATE_H
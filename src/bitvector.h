#ifndef BITVEC
#define BITVEC
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern BitVector* bv;

typedef struct {
    unsigned char* data;
    size_t size;
} BitVector;

void initBitVector(size_t numBits);
void setBit(int index);
void clearBit(int index);
int getBit(int index);
int getNextFreeSlot();
void freeVector();

#endif
#ifndef BITVEC
#define BITVEC
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
    unsigned char* data;
    size_t numBits;
    size_t size;
} BitVector;

extern BitVector* bv;

void initBitVector(size_t numBits);
void setBit(size_t index);
size_t getNextFreeSlot();
void freeBitVector();
static void clearBit(size_t index); // Not needed but since we don't free anything in the hack computer. It's implemented for learning purposes. 
static int getBit(size_t index);

#endif
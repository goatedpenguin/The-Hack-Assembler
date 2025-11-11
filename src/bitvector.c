#include "bitvector.h"
#include <stdio.h>
#include <stdlib.h>

extern BitVector* bv;

void initBitVector(size_t numBits) {
    int bytes = (numBits + 7) / 8;
    bv = (BitVector*)malloc(sizeof(BitVector));
    bv->data = (unsigned char*)calloc(bytes, sizeof(unsigned char));
    bv->numBits = numBits;
    bv->size = bytes;
}

void setBit(size_t index) {
    int byteIdx = index / 8;
    if (index < 0 || index >= bv->numBits)
        return;
    int bitPos = index % 8;
    unsigned char mask = 1 << bitPos;
    bv->data[byteIdx] |= mask;
    // It's possible to put 1 << bitPos where mask is, but its left
    // there for learning purposes... Done on the last line
}

static void clearBit(size_t index) {
    int byteIdx = index / 8;
    int bitPos = index % 8;
    if (index < 0 || index >= bv->numBits)
        return;
    // set to all 0s except one 1 in the mask, shift bitPos times and then
    // negate everything such that everything is 0xff except the 1 we shifted at
    // first
    bv->data[byteIdx] &= ~(1 << bitPos);
}

static int getBit(size_t index) {
    int byteIdx = index / 8;
    int bitPos = index % 8;
    if (index < 0 || index >= bv->numBits)
        return -1;
    // shift our target bits to the right by bitPos times, then AND it with one to preserve our target bit
    return (bv->data[byteIdx] >> bitPos) & 1;
}

size_t getNextFreeSlot() {
    for (size_t freeBitIndex = 0; freeBitIndex < bv->numBits; freeBitIndex++) {
        if (!getBit(freeBitIndex)) return freeBitIndex;
    }
    return (size_t) -1;
}

void freeBitVector() {
    free(bv->data);
    free(bv);
    bv = NULL;
}

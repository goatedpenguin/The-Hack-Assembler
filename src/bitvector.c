#include "bitvector.h"
#include <stdio.h>
#include <stdlib.h>

extern BitVector* bv;

void initBitVector(size_t numBits) {
    int bytes = (numBits + 7) / 8;
    bv = (BitVector* ) malloc(sizeof(BitVector));
    bv->data = (unsigned char *) calloc(bytes, sizeof(unsigned char));
    bv->size = numBits;
}   

void setBit(int index) {
    int byteIdx = index / 8;
    int bitPos = index % 8;
    unsigned char mask = 1 << bitPos;
    bv->data[byteIdx] |= mask; // It's possible to put 1 << bitPos where mask is, but its left there for learning purposes... Done in the next func
}

void clearBit(int index) {
    int byteIdx = index / 8;
    int bitPos = index % 8;
    // set to all 0s except one 1 in the mask, shift bitPos times and then negate everything such that everything is 0xff except the 1 we shifted at first
    bv->data[byteIdx] &= ~(1 << bitPos);  
}

int getBit(int index) {
    int byteIdx = index / 8;
    int bitPos = index % 8;

    
}

int getNextFreeSlot() {

}

void freeVector() {

}

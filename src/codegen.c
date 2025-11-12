#include "codegen.h"
#include "parser.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>

static char* decToBinStr(unsigned int dec) {
    int numBits = 15;                       
    char* binStr = (char *) malloc(numBits + 1); 
    int strSize = numBits - 1;              
    binStr[numBits] = '\0';                 

    for (int i = strSize; i >= 0; i--) {
        binStr[i] = (dec & 1) + '0';       
        dec >>= 1;  // % 2 but faster                        
    }

    return binStr;
}

char* symToBinStr(instrType instr, ParsedPacket* packet) {    
    char* res;
    switch (instr) {
        case A_INSTR: {
            res = decToBinStr(packet->value);
            return res;
        }
        case C_INSTR: {
            symTable* table = initTable();
            // load symbolic as key and its binary equivilent as the value 
            freeTable(table);
        }
    }
    
    return NULL;
}
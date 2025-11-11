#include "codegen.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

char* symToBinStr(instrType instr, ParsedPacket* packet) {
    int binNum = 0;
    char* strBinBuffer = (char *) malloc(16*sizeof(char));
    int bitPos = 0;
    switch (instr) {
        case A_INSTR: {
            while (packet->value != 0) {
                binNum |= (packet->value % 2) << bitPos;
                packet->value /= 2;
                bitPos++;
            }
            // build the string from msb to lsb
            return strBinBuffer;
        }
        case C_INSTR: {

        }
    }


    return binaryStr;  
}
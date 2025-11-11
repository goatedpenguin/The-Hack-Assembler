#include "parser.h"
#include "bitvector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern BitVector* bv;

static void stripWhiteSpace(char* line) {
    int i = 0;
    int len;
    len = strlen(line) + 1;
    while (line[i] != '\0') {
        if (line[i] == ' ') {
            len--;
            for (int j = i; j < len; j++) {
                line[j] = line[j + 1];
            }
            i--;
        }
        i++;
    }
}

static void stripComments(char* line) {
    int i = 0;
    while (line[i] != '\0') {
        if (line[i] == '/') {
            line[i] = '\0';
            break;
        }
        i++;
    }
}

void sanitizeLine(char* line) {
    stripComments(line);
    stripWhiteSpace(line);
}

static instrType detectInstrType(const char* line) {
    instrType type;

    if (line[0] == '@') {
        return A_INSTR;
    }

    if (line[0] == '(') {
        return L_INSTR;
    }

    return C_INSTR;
}

static char* extractSym(const char* line) {
    instrType type = detectInstrType(line);
    int len = strlen(line);
    char* sym = (char*)malloc(len);
    switch (type) {
    case A_INSTR:
        for (int i = 0; i < len; i++) {
            sym[i] = line[i + 1];
        }
        break;
    case L_INSTR:
        for (int i = 0; i < len; i++) {
            sym[i] = line[i + 1];
            if (line[i + 1] == ')') {
                sym[i] = '\0';
                break;
            }
        }
        break;
    }
    return sym;
}

static char* extractDest(const char* line) {
    int i = 0;
    int containsEqual = 0;
    char* res = malloc(8 * sizeof(char));

    while (line[i] != '\0') {
        if (line[i] == '=') {
            containsEqual = 1;
            break;
        }
        i++;
    }

    if (!containsEqual) {
        return NULL;
    }

    int j;
    for (j = 0; j < i; j++) {
        res[j] = line[j];
    }

    res[j] = '\0';
    return res;
}

static char* extractComp(const char* line) {
    char* comp = (char*)malloc(strlen(line) * sizeof(char) + 1);
    int start = -1, end = -1, i = 0;
    int tempIdx = 0;

    while (line[i]) {
        if (line[i] == '=') {
            start = i + 1;
        }
        if (line[i] == ';') {
            end = i;
        }
        i++;
    }

    if (start == -1 && end == -1) {
        strcpy(comp, line);
    } else if (start != -1 && end == -1) {
        for (int i = start; i < strlen(line); i++) {
            comp[tempIdx] = line[i];
            tempIdx++;
        }
        comp[tempIdx] = '\0';
    } else if (start == -1 && end != -1) {
        for (int i = 0; i < end; i++) {
            comp[tempIdx] = line[i];
            tempIdx++;
        }
        comp[tempIdx] = '\0';
    } else {
        for (int i = start; i < end; i++) {
            comp[tempIdx] = line[i];
            tempIdx++;
        }
        comp[tempIdx] = '\0';
    }

    return comp;
}

static char* extractJump(const char* line) {
    int i = 0;
    int containsSemicolon = 0;
    char* res = malloc(8 * sizeof(char));

    while (line[i] != '\0') {
        if (line[i] == ';') {
            containsSemicolon = 1;
            break;
        }
        i++;
    }

    if (!containsSemicolon) {
        return NULL;
    }

    int idx = 0;
    int j = i + 1;
    while (line[j] != '\0') {
        res[idx++] = line[j++];
    }

    res[idx] = '\0';
    return res;
}

ParsedPacket* parseCInstruction(const char* line) {
    ParsedPacket* packet = (ParsedPacket*)malloc(sizeof(ParsedPacket));

    packet->comp = extractComp(line);
    packet->dest = extractDest(line);
    packet->jump = extractJump(line);

    return packet;
}

ParsedPacket* parseAInstruction(symTable* table, const char* line) {
    // check if it overflows to screen memory region


    ParsedPacket* packet = (ParsedPacket*)malloc(sizeof(ParsedPacket));
    // detect if a instr is binded to a label, is a var, or a numeric

    char* symbol = extractSym(line);
    int retval = getSym(table, symbol);

    // label processing
    if (retval != -1) {
        packet->value = retval;
        return packet;
    }

    // else, check the string to see whether its a numeric or a var e.x. "5" vs
    // "i" check numeric first if not, then it must be a var:

    if (symbol[0] >= '0' && symbol[0] <= '9') {
        int val = atoi(symbol);
        packet->value = val;
        return packet;
    } 

    // bit vector collision and resolve logic goes here
    size_t freeRamAddr = getNextFreeSlot() + (size_t) 16;
    if (freeRamAddr > 16383) return NULL; // to prevent overflow in to screen region memory
    setBit(freeRamAddr); // mark as used
    packet->value = freeRamAddr; // hack addr only go up till 24576, way less than 2^16. Typecast from size_t -> uint16_t is safe here 
    
    free(symbol);
    return packet;
}

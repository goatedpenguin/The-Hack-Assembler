#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"


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

void santizeLine(char* line) {
    stripComments(line);
    stripWhiteSpace(line);
}

instrType detectInstrType(const char* line) {
    instrType type;
    
    if (line[0] == '@') {
        return A_INSTR;
    }

    if (line[0] == '(') {
        return L_INSTR;
    }

    return C_INSTR;
}

char* extractSym(const char* line) {
    instrType type = detectInstrType(line);
    int len = strlen(line);
    char* sym = (char *) malloc(len);
    switch (type) {
        case 0:
            for (int i = 0; i < len; i++) {
                sym[i] = line[i + 1];
            }
            break;   
        case 2:
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

char* extractDest(char* line) {
    int i = 0;
    int containsEqual = 0;
    char* res = malloc(8*sizeof(char));

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

char* extractComp(char* line) {
    
}

char* extractJump(char* line) {
    int i = 0;
    int containsSemicolon = 0;
    char* res = malloc(8*sizeof(char));

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


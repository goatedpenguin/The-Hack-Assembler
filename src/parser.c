#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"


static void stripWhiteSpace(char* line) {
    int i = 0;
    int len;
    len = strlen(line) + 1;
    while(line[i] != '\0') {
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
    while(line[i] != '\0') {
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
    char* sym = (char *) malloc(len);
    switch (type) {
        case A_INSTR:
            for(int i = 0; i < len; i++) {
                sym[i] = line[i + 1];
            }
            break;   
        case L_INSTR:
            for(int i = 0; i < len; i++) {
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

static char* extractDest(char* line) {
    int i = 0;
    int containsEqual = 0;
    char* res = malloc(8*sizeof(char));

    while(line[i] != '\0') {
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

static char* extractComp(char* line) {
    char* comp = (char *) malloc(strlen(line)*sizeof(char) + 1);
    int start = -1, end = -1, i = 0;
    int tempIdx = 0;

    while(line[i]) {
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
        for(int i = start; i < strlen(line); i++) {
            comp[tempIdx] = line[i];
            tempIdx++;
        }
        comp[tempIdx] = '\0';
    } else if (start == -1 && end != -1) {
        for(int i = 0; i < end; i++) {
            comp[tempIdx] = line[i];
            tempIdx++;
        }
        comp[tempIdx] = '\0';
    }
    else {
        for(int i = start; i < end; i++) {
            comp[tempIdx] = line[i];
            tempIdx++;
        }
        comp[tempIdx] = '\0';
    }

    return comp;
}

static char* extractJump(char* line) {
    int i = 0;
    int containsSemicolon = 0;
    char* res = malloc(8*sizeof(char));

    while(line[i] != '\0') {
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
    while(line[j] != '\0') {
        res[idx++] = line[j++];
    }

    res[idx] = '\0';
    return res;
}


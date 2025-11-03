#ifndef PARSER
#define PARSER
#include <stdio.h>

typedef enum {
    A_INSTR,
    C_INSTR,
    L_INSTR
} instrType;

typedef struct parsedPacket {
    instrType type;
    char* symbol;
    int aBit;
    char comp[8];
    char dest[8];
    char jump[8];
} ParsedPacket;

static void stripComments(char* line);
static void stripWhiteSpace(char* line);
void santizeLine(char* line);

instrType detectInstrType(const char* line);

char* extractSym(const char* line);
char* extractDest(char* line);
char* extractComp(char* line);
char* extractJump(char* line);

#endif
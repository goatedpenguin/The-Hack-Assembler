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
void sanitizeLine(char* line);

static instrType detectInstrType(const char* line);

static char* extractSym(const char* line);
static char* extractDest(char* line);
static char* extractComp(char* line);
static char* extractJump(char* line);

ParsedPacket buildPacket(symTable* table, const char* line)

#endif
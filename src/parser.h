#ifndef PARSER
#define PARSER
#include "symbol_table.h"
#include <stdint.h>
#include <stdio.h>

typedef enum { A_INSTR, C_INSTR, L_INSTR } instrType;

typedef struct parsedPacket {
    uint16_t value; // numeric val of a instr
    char* symbol; // for dbg
    char* comp;
    char* dest;
    char* jump;
} ParsedPacket;

static void stripComments(char* line);
static void stripWhiteSpace(char* line);
void sanitizeLine(char* line);

static instrType detectInstrType(const char* line);

static char* extractSym(const char* line);
static char* extractDest(const char* line);
static char* extractComp(const char* line);
static char* extractJump(const char* line);

ParsedPacket* parseCInstruction(const char* line);
ParsedPacket* parseAInstruction(symTable* table, const char* line);

#endif
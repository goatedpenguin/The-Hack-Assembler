#define _GNU_SOURCE
#include "assembler.h"
#include "bitvector.h"
#include "codegen.h"
#include "parser.h"
#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

extern FILE* file;
extern BitVector* bv;

void assemblerFirstPass(symTable* table) {
    loadPredefinedSymbols(table);
    char* line = NULL;
    char* ogLine;
    ssize_t read = 0;
    size_t len = 0;
    int romAddr = 0;
    instrType instr;

    while ((read = getline(&line, &len, file)) != -1) {
        sanitizeLine(line);

        if (line[0] == '\0')
            continue;

        instr = detectInstrType(line);
        switch (instr) {
        case A_INSTR: {
            if (line[1] >= '0' && line[1] <= '9') {
                char* strAddr = extractSym(line);
                int addr = atoi(strAddr);
                setBit(addr);
            }
            break;
        }
        case C_INSTR:
            romAddr++;
            break;
        case L_INSTR: {
            char* sym = extractSym(line);
            addSym(table, sym, romAddr);
            free(sym);
            break;
        }
        default:
            fprintf(stderr, "Invalid instruction: %s\n", line);
            free(line);
            exit(EXIT_FAILURE);
            break;
        }
    }
    free(line);
}

void assemblerSecondPass(const char* programExecutableName, symTable* table) {
    rewind(file);
    char* line = NULL;
    ssize_t read = 0;
    size_t len = 0;
    instrType instr;
    char* outputBin;

    size_t bufferSize = strlen(programExecutableName) + strlen(".hack") + 1;
    char* programExecutableNameBuffer = (char*)malloc(bufferSize);
    sprintf(programExecutableNameBuffer, "%s.hack", programExecutableName);
    FILE* program = fopen(programExecutableNameBuffer, "w");
    free(programExecutableNameBuffer);

    while ((read = getline(&line, &len, file)) != -1) {
        instr = detectInstrType(line);
        switch (instr) {
        case A_INSTR:
        case C_INSTR: {
            ParsedPacket* packet = fetchInstrPacket(table, line);
            outputBin = symToBinStr(instr, packet);
            fprintf(program, "%s\n", outputBin);
            free(packet);
            free(outputBin);
            break;
            }
        }
    }
    freeTable(table);
    free(line);
    fclose(program);
}

void loadPredefinedSymbols(symTable* table) {
    char RSym[4];

    for (int i = 0; i < 16; i++) {
        sprintf(RSym, "R%d", i);
        addSym(table, strdup(RSym), i);
    }

    addSym(table, "SCREEN", 16384);
    addSym(table, "KBD", 24576);
    addSym(table, "SP", 0);
    addSym(table, "LCL", 1);
    addSym(table, "ARG", 2);
    addSym(table, "THIS", 3);
    addSym(table, "THAT", 4);
}

ParsedPacket* fetchInstrPacket(symTable* table, const char* line) {
    instrType instr;
    instr = detectInstrType(line);
    ParsedPacket* packet;

    switch (instr) {
    case A_INSTR: {
        packet = parseAInstruction(table, line);
        return packet;
    }

    case C_INSTR: {
        packet = parseCInstruction(line);
        return packet;
    }
    }
    return NULL;
}

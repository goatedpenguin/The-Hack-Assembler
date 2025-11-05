
#ifndef ASSEMBLER
#define ASSEMBLER
#include "parser.h"
#include "symbol_table.h"


// load file
// santize it
// populate default symbols in a seperte table and make it const
// init symbol table
// fill symbol table with all the labels
// close the file
// ram addr assignment starts from addr 16 and above (0-15)

// code gen then fetches the instr packets and looks up stuff from the symbol table if its an instruction
    // if a is a label then we put in the addr for that to binary
    // if a is a variable(symbolic(letters)) then we put it in the next available ram slot
    // if a is a number directly convert to binary

void phase1(symTable* table, const char* filename);
void phase2(symTable* table, const char* filename);


#endif


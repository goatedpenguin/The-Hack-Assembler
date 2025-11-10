#include "assembler.h"
#include "bitvector.h"
#include "codegen.h"
#include "parser.h"
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

BitVector* bv = NULL;
FILE* file = NULL;

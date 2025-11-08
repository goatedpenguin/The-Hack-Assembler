#ifndef CODEGEN
#define CODEGEN
#include "parser.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char* symToBinStr(ParsedPacket* packet);
uint16_t symTOBinB(ParsedPacket* packet);

#endif
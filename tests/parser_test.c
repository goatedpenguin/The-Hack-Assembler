// test_parser_with_expected.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../src/parser.h"

static void print_sep(void) {
    printf("----------------------------------------\n");
}

// Utility to compare strings safely
static int str_eq(const char* a, const char* b) {
    if (!a && !b) return 1;
    if (!a || !b) return 0;
    return strcmp(a, b) == 0;
}

static void run_case(const char *raw_line, const char* exp_sym,
                     const char* exp_dest, const char* exp_comp, const char* exp_jump) {
    char linebuf[256];
    strncpy(linebuf, raw_line, sizeof(linebuf)-1);
    linebuf[sizeof(linebuf)-1] = '\0';

    sanitizeLine(linebuf);

    instrType t = detectInstrType(linebuf);
    char *sym = NULL, *dest = NULL, *comp = NULL, *jump = NULL;

    switch (t) {
    case A_INSTR:
        sym = extractSym(linebuf);
        break;
    case L_INSTR:
        sym = extractSym(linebuf);
        break;
    case C_INSTR:
        dest = extractDest(linebuf);
        comp = extractComp(linebuf);
        jump = extractJump(linebuf);
        break;
    }

    int pass = 1;
    if (!str_eq(sym, exp_sym)) pass = 0;
    if (!str_eq(dest, exp_dest)) pass = 0;
    if (!str_eq(comp, exp_comp)) pass = 0;
    if (!str_eq(jump, exp_jump)) pass = 0;

    printf("Test: \"%s\" -> %s\n", raw_line, pass ? "TRUE" : "FALSE");
    if (!pass) {
        printf("  Expected: sym='%s', dest='%s', comp='%s', jump='%s'\n",
               exp_sym ? exp_sym : "(null)",
               exp_dest ? exp_dest : "(null)",
               exp_comp ? exp_comp : "(null)",
               exp_jump ? exp_jump : "(null)");
        printf("  Got     : sym='%s', dest='%s', comp='%s', jump='%s'\n",
               sym ? sym : "(null)",
               dest ? dest : "(null)",
               comp ? comp : "(null)",
               jump ? jump : "(null)");
    }

    free(sym); free(dest); free(comp); free(jump);
    print_sep();
}

int main(void) {
    struct {
        const char* line;
        const char* sym;
        const char* dest;
        const char* comp;
        const char* jump;
    } tests[] = {
        {"@i",        "i",     NULL,   NULL,   NULL},
        {"@123",      "123",   NULL,   NULL,   NULL},
        {"(LOOP)",    "LOOP",  NULL,   NULL,   NULL},
        {"D=M",       NULL,    "D",    "M",    NULL},
        {"MD=D-1",    NULL,    "MD",   "D-1",  NULL},
        {"A=D+1;JGE", NULL,    "A",    "D+1",  "JGE"},
        {"0;JMP",     NULL,    NULL,   "0",    "JMP"},
        {"D;JEQ",     NULL,    NULL,   "D",    "JEQ"},
        {";JMP",      NULL,    NULL,   "",     "JMP"},
        {"M=1",       NULL,    "M",    "1",    NULL},
        {"@",         "",      NULL,   NULL,   NULL},
        {"()",        "",      NULL,   NULL,   NULL},
        {NULL,        NULL,    NULL,   NULL,   NULL} // sentinel
    };

    printf("Running parser tests with expected outputs\n");
    print_sep();

    for (int i = 0; tests[i].line != NULL; i++) {
        run_case(tests[i].line, tests[i].sym, tests[i].dest, tests[i].comp, tests[i].jump);
    }

    return 0;
}

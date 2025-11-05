#include <stdio.h>
#include <assert.h>
#include "symbol_table.h"

int main(void) {
    printf("=== Symbol Table Test Start ===\n");

    printf("[1] Initializing table...\n");
    symTable* table = initTable();
    assert(table != NULL);
    printf("    -> Initial capacity: %d\n", table->capacity);
    printf("    -> Initial size: %d\n", table->size);

    printf("[2] Adding initial symbols...\n");
    addSym(table, "LOOP", 4);
    addSym(table, "END", 10);
    addSym(table, "sum", 16);
    printf("    -> Added LOOP=4, END=10, sum=16\n");

    printf("[3] Verifying lookups...\n");
    assert(getSym(table, "LOOP") == 4);
    assert(getSym(table, "END") == 10);
    assert(getSym(table, "sum") == 16);
    assert(getSym(table, "NOTFOUND") == -1);
    printf("    -> Lookup tests passed\n");

    printf("[4] Testing overwrite behavior...\n");
    addSym(table, "LOOP", 99);
    int val = getSym(table, "LOOP");
    printf("    -> LOOP now points to: %d\n", val);

    printf("[5] Stress testing resize with many symbols...\n");
    for (int i = 0; i < 2000; i++) {
        char key[32];
        sprintf(key, "SYM%d", i);
        addSym(table, key, i);
        if (i % 400 == 0) printf("    -> Inserted %d symbols...\n", i);
    }

    printf("[6] Checking post-resize integrity...\n");
    assert(getSym(table, "SYM0") == 0);
    assert(getSym(table, "SYM1999") == 1999);
    printf("    -> Lookups OK after resize\n");

    printf("[7] Current stats:\n");
    printf("    -> Table size: %d\n", table->size);
    printf("    -> Table capacity: %d\n", table->capacity);

    printf("[8] Cleaning up...\n");
    freeTable(table);

    printf("=== All symbol table tests completed successfully ===\n");
    return 0;
}

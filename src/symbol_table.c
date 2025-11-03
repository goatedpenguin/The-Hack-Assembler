#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

#define INIT_TABLE_SIZE 512


symTable* initTable(void) {
    symTable* emptyTable = (symTable*) malloc(sizeof(symTable));
    emptyTable->capacity = INIT_TABLE_SIZE;
    emptyTable->size = 0;
    emptyTable->entries = (entry **) malloc(emptyTable->capacity * sizeof(entry*));
    
    for (int i = 0; i < emptyTable->capacity; i++) {
        emptyTable->entries[i] = NULL;
    }

    return emptyTable;
}

void freeTable(symTable* table) {
    entry* temp;
    entry* prev;
    for (int i = 0; i < table->capacity; i++) {
        temp = table->entries[i];
        prev = table->entries[i];
        while (temp != NULL) {
            free(temp->name);
            temp = temp->next;
            free(prev);
            prev = temp;
        }
    }
    free(table->entries);
    free(table);
}

unsigned long hash(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;  
    return hash;
}

void addSym(symTable* table, const char* name, int address) {
    int newIdx;
    if (table->size >= (int) (2.0 / 3 * table->capacity)) {
        // rehash everything here
        for (int i = 0; i < table->capacity; i++) {
            newIdx = hash(table->entries[i]->name);
            if (table->entries[newIdx] != NULL) {
                // transfer whole chain to new index and check if that has anything
            } else {
                table->entries[newIdx] = table->entries[i];
            }
        }
        table->entries = (entry**) realloc(table->entries, table->capacity * 2 * sizeof(entry*));
        table->capacity = (int) table->capacity * 2;
    }

    int idx = hash(name) % table->capacity;

    entry* temp = (entry *) malloc(sizeof(entry));

    temp->address = address;
    temp->name = strdup(name);
    temp->next = table->entries[idx];
    table->entries[idx] = temp;
    table->size++; 
}

int getSym(symTable* table, const char* name) {
    int idx = hash(name) % table->capacity;
    entry* temp = table->entries[idx];
    
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            return temp->address;
        }
        temp = temp->next; 
    }
    return -1;
}
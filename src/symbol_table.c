#define _POSIX_C_SOURCE 200809L
#include "symbol_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_TABLE_SIZE 512

// MAKE IT A POWER OF 2!!!

symTable* initTable(void) {
    symTable* emptyTable = (symTable*)malloc(sizeof(symTable));
    emptyTable->capacity = INIT_TABLE_SIZE;
    emptyTable->size = 0;
    emptyTable->entries =
        (entry**)malloc(emptyTable->capacity * sizeof(entry*));

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

void resize(symTable* table) {
    int newIdx;
    int oldCapacity = table->capacity;
    entry* temp;
    entry* next;
    if (table->size * 3 >= 2 * table->capacity) {
        table->entries = (entry**)realloc(
            table->entries, table->capacity * 2 * sizeof(entry*));
        table->capacity = (int)table->capacity * 2;

        for (int i = oldCapacity; i < table->capacity; i++) {
            table->entries[i] = NULL;
        }

        for (int i = 0; i < oldCapacity; i++) {
            temp = table->entries[i];
            while (temp != NULL) {
                newIdx = hash(temp->name) % table->capacity;
                next = temp->next;
                temp->next = table->entries[newIdx];
                table->entries[newIdx] = temp;
                temp = next;
            }
        }
    }
}

void addSym(symTable* table, const char* name, int address) {
    resize(table);
    int idx = hash(name) % table->capacity;

    entry* temp = (entry*)malloc(sizeof(entry));

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
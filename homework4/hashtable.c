#include "hashtable.h"
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "hash.h"
#include <stdint.h>
#include <string.h>
#define MAX_KEY_LENGTH 256

typedef struct hashtable_entry {
    char *key;
    int n;
} hashtable_entry_t;

typedef struct hashtable {
    hashtable_entry_t *mainTable;
    int distinctKeyCount;
    int tableSize;
} hashtable_t;

hashtable_t *hashtable_create(int tableSize) {
    hashtable_t *ht = calloc(1, sizeof(hashtable_t));
    ht->tableSize = tableSize;
    ht->mainTable = calloc(ht->tableSize, sizeof(hashtable_entry_t));
    
    return ht;
}

void hashtable_set(hashtable_t *ht, char *key, int value) {
    int hashVal = fibonacci32_reduce(fxhash32_hash((uint8_t *)key,
                                     strlen(key)), (int)log2(ht->tableSize));
    if (ht->distinctKeyCount / (double)ht->tableSize >= 0.5) {
        //print no. of collisions before

        hashtable_grow(ht);
        //print no. of collisions after
    }
    while (true) {
        //if slot empty
        if (!ht->mainTable[hashVal].key) {
            ht->mainTable[hashVal].key = strdup(key);
            ht->mainTable[hashVal].n = value;
            ht->distinctKeyCount++;
            return;
        }
        //if the strings are different
        if (!strcmp(key, ht->mainTable[hashVal].key)) {
            ht->mainTable[hashVal].n = value;
            return;
        }
        hashVal++;
    }
}

bool hashtable_get(hashtable_t *ht, char *key, int *value) {
    int hashVal = fibonacci32_reduce(fxhash32_hash((uint8_t *)key,
                                     strlen(key)), (int)log2(ht->tableSize));
    while (true) {
        if (!ht->mainTable[hashVal].key) {
            return false;
        }
        if (!strcmp(key, ht->mainTable[hashVal].key)) {
            *value = ht->mainTable[hashVal].n;
            return true;
        }
        hashVal++;
    }
}

void hashtable_grow(hashtable_t *ht) {
    int newTableSize = ht->tableSize * 2;
    hashtable_t *newHt = hashtable_create(newTableSize);
    for (int i = 0; i < ht->tableSize; i++) {
        if (ht->mainTable[i].n != 0) {
            hashtable_set(newHt, ht->mainTable[i].key, ht->mainTable[i].n);
        }
    }
    hashtable_destroy(ht);
    ht = newHt;
}

int hashtable_size(hashtable_t *ht) {
    return ht->distinctKeyCount;
}

int hashtable_probe_max(hashtable_t *ht) {
    return ht->tableSize;
}

bool hashtable_probe(hashtable_t *ht, int i, char **key, int *val) {
    if (!ht->mainTable[i].key) {
        return false;
    }
    *key = ht->mainTable[i].key;
    *val = ht->mainTable[i].n;
    return true;
}

void hashtable_destroy(hashtable_t *ht) {
    free(ht->mainTable);
    free(ht);
    for ()
}

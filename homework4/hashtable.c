#include "hashtable.h"
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "hash.h"
#include <stdint.h>
#include <string.h>

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
    while (true) {
        if (!ht->mainTable[hashVal].key) {
            ht->mainTable[hashVal].key = key;
            ht->mainTable[hashVal].n = value;
            ht->distinctKeyCount++;
            return;
        }
        if (key == ht->mainTable[hashVal].key) {
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
        if (key == ht->mainTable[hashVal].key) {
            *value = ht->mainTable[hashVal].n;
            return true;
        }
        hashVal++;
    }
}
//todo
// void hashtable_grow(hashtable_t *ht) {
//     int newTableSize = ht->tableSize * 2;
//     hashtable_t *newHt = hashtable_create(newTableSize);
//     for (int i = 0; i < ht->tableSize; i++) {
//         if (ht->mainTable[i].key != NULL) {
//             hashtable_set(newHt, , )
//         }
//     }
// }

void hashtable_destroy(hashtable_t *ht) {
    free(ht->mainTable);
    free(ht);
}

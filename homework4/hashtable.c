#define _GNU_SOURCE
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
    int val;
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

void inner_destroy(hashtable_t *ht) {
    for (int i = 0; i < ht->tableSize; i++) {
        if(ht->mainTable[i].key) {
            free(ht->mainTable[i].key);
        }
    }
    free(ht->mainTable);
}

int evaluate_collision_count(hashtable_t *ht) {
    int hashVals[ht->tableSize];
    for (int i = 0; i < ht->tableSize; i++) {
        hashVals[i] = 0;
    }
    int collisionCount = 0;
        for (int j = 0; j < ht->tableSize; j++) {
            if (ht->mainTable[j].key) {
                uint32_t hash = fibonacci32_reduce(fxhash32_hash((uint8_t *)ht->mainTable[j].key, strlen(ht->mainTable[j].key)), (int)log2(ht->tableSize));
                hashVals[hash]++;
            }
        }
    for (int i = 0; i < ht->tableSize; i++) {
        if (hashVals[i] > 1) {
            collisionCount += hashVals[i] - 1;
        }
    }
    return collisionCount;
}

void hashtable_set(hashtable_t *ht, char *key, int value) {

    if (ht->distinctKeyCount / (double)ht->tableSize >= 0.5) {
        int prevCollisions = evaluate_collision_count(ht);
        hashtable_grow(ht);
        int postCollisions = evaluate_collision_count(ht);
        printf("Rehashing reduced collisions from %d to %d\n", prevCollisions, postCollisions);
    }
    int hashVal = fibonacci32_reduce(fxhash32_hash((uint8_t *)key,
                                     strlen(key)), (int)log2(ht->tableSize));
    while (true) {
        //if slot empty
        if (!ht->mainTable[hashVal].key) {
            ht->mainTable[hashVal].key = strdup(key);
            ht->mainTable[hashVal].val = value;
            ht->distinctKeyCount++;
            return;
        }
        //if the strings are different
        if (!strcmp(key, ht->mainTable[hashVal].key)) {
            ht->mainTable[hashVal].val = value;
            return;
        }
        hashVal = (hashVal + 1) % ht->tableSize;
    }
}

bool hashtable_get(hashtable_t *ht, char *key, int *value) {
    int hashVal = fibonacci32_reduce(fxhash32_hash((uint8_t *)key,
                                     strlen(key)), (int)log2(ht->tableSize));
    int count = 0;
    while (true) {
        if (!ht->mainTable[hashVal].key) {
            return false;
        }
        if (!strcmp(key, ht->mainTable[hashVal].key)) {
            *value = ht->mainTable[hashVal].val;
            return true;
        }
        hashVal = (hashVal + 1) % ht->tableSize;
    }
}

void hashtable_grow(hashtable_t *ht) {
    int newTableSize = ht->tableSize * 2;
    hashtable_t *newHt = hashtable_create(newTableSize);
    for (int i = 0; i < ht->tableSize; i++) {
        if (ht->mainTable[i].key) {
            hashtable_set(newHt, ht->mainTable[i].key, ht->mainTable[i].val);
        }
    }
    inner_destroy(ht);
    ht->distinctKeyCount = newHt->distinctKeyCount;
    ht->mainTable = newHt->mainTable;
    ht->tableSize = newHt->tableSize;
    free(newHt);

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
    *val = ht->mainTable[i].val;
    return true;
}

void hashtable_destroy(hashtable_t *ht) {
    inner_destroy(ht);
    free(ht);
}

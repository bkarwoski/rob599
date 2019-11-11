#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "hash.h"
#define TABLE_SIZE 8192
#define MAX_LINE_LENGTH 256

typedef struct test_entry {
    uint8_t *data;
    int n;
} test_entry_t;

void evaluate_hash_reduce(int n_entries, test_entry_t *entries,
                          uint32_t (*hash_f)(uint8_t *, int), uint32_t (*reduce_f)(uint32_t)) {
    int hashVals[TABLE_SIZE];
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashVals[i] = 0;
    }
    clock_t start = clock();
    int collisionCount = 0;
    int loopCount = 0;
    while (((double)clock() - (double)start) / (double)CLOCKS_PER_SEC < 0.5) {
        for (int j = 0; j < n_entries; j++) {
            uint32_t hash = reduce_f(hash_f(entries[j].data, entries[j].n));
            if (loopCount == 0) {
                hashVals[hash]++;
            }
        }
        loopCount++;
    }
    clock_t end = clock();
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashVals[i] > 1) {
            collisionCount += hashVals[i] - 1;
        }
    }
    double timePerLoop = ((double)end - (double)start) /
                        (double)CLOCKS_PER_SEC / loopCount * 1e9 / n_entries;
    printf("%.2fns per iteration with %d collisions\n", timePerLoop, collisionCount);
}

uint32_t modulo2_reduce(uint32_t hash) {
    return hash & (TABLE_SIZE - 1);
}

uint32_t modulo_prime_reduce(uint32_t hash) {
    return hash % 8191;
}

uint32_t fibonacci32_reduce(uint32_t hash) {
    const uint32_t factor32 = 2654435769;
    return (uint32_t)(hash * factor32) >> (32 - 13);
}

int main(int argc, char **argv) {
    int n_entries = 0;
    //bool fast_mode = 0;
    FILE *f = fopen("book.txt", "r");
    setup_table_hash();
    // First we need to collect all the entries/strings/data that we will try to hash
    // This needs to be done up front for the benchmarking later to be valid.
    uint16_t max_entries = TABLE_SIZE / 2;
    test_entry_t *entries = calloc(max_entries, sizeof(test_entry_t));
    for (uint16_t i = 0; i < 1000; i++) {
        entries[i].data = malloc(sizeof(i));
        memcpy(entries[i].data, &i, sizeof(i));
        entries[i].n = sizeof(i);
        n_entries++;
    }
    for (uint16_t i = 1000; i < max_entries; i++) {
        char nextStr[MAX_LINE_LENGTH];
        if (*fgets(nextStr, MAX_LINE_LENGTH, f) == '\0') {
            printf("error, end of line on fgets\n");
        }
        entries[i].data = (uint8_t *)strdup(nextStr);
        entries[i].n = (int)strlen(nextStr);
        n_entries++;
    }
    uint32_t (*hash_functions[])(uint8_t *, int) = {add_hash, table_a_hash, table_b_hash,
                                                    djb2a_hash, fnv1a_hash, fxhash32_hash};
    int n_hash_functions = sizeof(hash_functions) / sizeof(hash_functions[0]);

    uint32_t (*reduce_functions[])(uint32_t) = {modulo2_reduce, modulo_prime_reduce,
                                                fibonacci32_reduce};
    int n_reduce_functions = sizeof(reduce_functions) / sizeof(reduce_functions[0]);

    for (int hash_i = 0; hash_i < n_hash_functions; hash_i++) {
        for (int reduce_i = 0; reduce_i < n_reduce_functions; reduce_i++) {
            evaluate_hash_reduce(n_entries, entries, hash_functions[hash_i],
                                 reduce_functions[reduce_i]);
        }
        printf("\n");
    }
    fclose(f);
    for (int i = 0; i < max_entries; i++) {
        free(entries[i].data);
    }
    free(entries);

    return 0;
}

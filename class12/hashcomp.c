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
    int loopCount = 100;
    for (int i = 0; i < loopCount; i++) {
        for (int j = 0; j < n_entries; j++) {
            uint32_t hash = reduce_f(hash_f(entries[j].data, entries[j].n));
            hashVals[hash]++;
        }
    }
    clock_t end = clock();
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashVals[i] > 1) {
            collisionCount++;
        }
    }
    double timePerLoop = (end - start) / loopCount;
    printf("%.2fns per iteration with %d collisions\n",timePerLoop, collisionCount);
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
    bool fast_mode = 0;
    FILE *f;
    f = fopen("book.txt", "r");

    // First we need to collect all the entries/strings/data that we will try to hash
    // This needs to be done up front for the benchmarking later to be valid.
    int max_entries = TABLE_SIZE / 2;
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
        entries[i].n = strlen(nextStr);
        n_entries++;
    }
    // This is an array of function pointers
    // Which will let us iterate through all the different hash functions
    // The syntax is actually only slightly different from a normal function
    // for a function pointer, the name of the function is in parenthesis
    // and it has an asterisk in front of it. The brackets after the name
    // show it is an array of function pointers. Everything outside those parentheses
    // work like normal for the function definition, and the arguments don't need names.
    uint32_t (*hash_functions[])(uint8_t *, int) = {add_hash, table_a_hash, table_b_hash,
                                                    djb2a_hash, fnv1a_hash, fxhash32_hash};
    // This is a good idiom to get the number of elements in a static array in C
    // This only works because the hash function array is declared as a literal array with { ... }
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

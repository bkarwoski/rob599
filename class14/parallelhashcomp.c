#define _GNU_SOURCE
#include <pthread.h>
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

typedef struct thread_info {
    pthread_t thread;
    int n_entries;
    test_entry_t *entries;
    uint32_t (*hash_f)(uint8_t *data, int n);
    uint32_t (*reduce_f)(uint32_t hash);
    double timePerLoop;
    int collisionCount;
} thread_info_t;

double seconds_now(void) {
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now)) {
        fprintf(stderr, "Retrieving system time failed.\n");
        exit(1);
    }
    return now.tv_sec + now.tv_nsec / 1000000000.0;
}

void evaluate_hash_reduce(thread_info_t *info, int n_entries, test_entry_t *entries,
                          uint32_t (*hash_f)(uint8_t *, int), uint32_t (*reduce_f)(uint32_t)) {
    int hashVals[TABLE_SIZE];
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashVals[i] = 0;
    }
    double start = seconds_now();
    int collisionCount = 0;
    int loopCount = 0;
    while (seconds_now() - start < 0.5) {
        for (int j = 0; j < n_entries; j++) {
            uint32_t hash = reduce_f(hash_f(entries[j].data, entries[j].n));
            if (loopCount == 0) {
                hashVals[hash]++;
            }
        }
        loopCount++;
    }
    double end = seconds_now();
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashVals[i] > 1) {
            collisionCount += hashVals[i] - 1;
        }
    }
    double timePerLoop = (end - start) / loopCount / n_entries * 1e9;
    info->collisionCount = collisionCount;
    info->timePerLoop = timePerLoop;
}

void *thread_start(void *user) {
    thread_info_t *info = user;
    evaluate_hash_reduce(info, info->n_entries, info->entries, info->hash_f, info->reduce_f);
    //printf("Hello world from %d!\n", info->num);
    return NULL;
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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <number of threads>\n", argv[0]);
        return 1;
    }
    int nThreads = atoi(argv[1]);
    if (nThreads <= 0) {
        printf("error, need positive number of threads\n");
        return 1;
    }
    int n_entries = 0;
    FILE *f = fopen("book.txt", "r");
    setup_table_hash();
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
    thread_info_t *thread_infos = calloc(nThreads, sizeof(thread_info_t));
    int activeThreadCount = 0;
    for (int hash_i = 0; hash_i < n_hash_functions; hash_i++) {
        for (int reduce_i = 0; reduce_i < n_reduce_functions; reduce_i++) {
            thread_infos[activeThreadCount].n_entries = n_entries;
            thread_infos[activeThreadCount].entries = entries;
            thread_infos[activeThreadCount].hash_f = hash_functions[hash_i];
            thread_infos[activeThreadCount].reduce_f = reduce_functions[reduce_i];

            pthread_create(&thread_infos[activeThreadCount].thread, NULL, thread_start,
                           &thread_infos[activeThreadCount]);
            activeThreadCount++;
            if (activeThreadCount >= nThreads || (hash_i == n_hash_functions - 1 &&
                                                  reduce_i == n_reduce_functions - 1)) {
                //printf("threads filled\n");
                for (int z = 0; z < activeThreadCount; z++) {
                    pthread_join(thread_infos[z].thread, NULL);
                    printf("%.2fns per iteration with %d collisions\n",
                           thread_infos[z].timePerLoop, thread_infos[z].collisionCount);
                }
                activeThreadCount = 0;
            }
        }
        printf("\n");
    }
    free(thread_infos);
    fclose(f);
    for (int i = 0; i < max_entries; i++) {
        free(entries[i].data);
    }
    free(entries);

    return 0;
}

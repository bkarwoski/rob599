#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
uint32_t table_hash_keys[256];

uint32_t fibonacci32_reduce(uint32_t hash, int bitCount) {
    const uint32_t factor32 = 2654435769;
    return (uint32_t)(hash * factor32) >> (32 - bitCount);
}

uint32_t add_hash(uint8_t *data, int n) {
    uint32_t hash = 0;
    for (int i = 0; i < n; i++) {
        hash += *data;
        data++;
    }
    return hash;
}

void setup_table_hash(void) {
    for (int i = 0; i < 256; i++) {
        table_hash_keys[i] = rand();
    }
}

uint32_t table_a_hash(uint8_t *data, int n) {
    uint32_t hash = 0;
    for (int i = 0; i < n; i++) {
        hash += table_hash_keys[*data];
        data++;
    }
    return hash;
}

uint32_t table_b_hash(uint8_t *data, int n) {
    uint32_t hash = 0;
    for (int i = 0; i < n; i++) {
        hash = (hash << 3) ^ table_hash_keys[*data];
        data++;
    }
    return hash;
}

uint32_t djb2a_hash(uint8_t *data, int n) {
    uint32_t hash = 5381;
    for (int i = 0; i < n; i++) {
        hash = ((hash << 5) + hash) ^ *data;
        data++;
    }
    return hash;
}

uint32_t fnv1a_hash(uint8_t *data, int n) {
    uint32_t hash = 2166136261;
    uint32_t fnvPrime = 16777619;
    for (int i = 0; i < n; i++) {
        hash = hash ^ *data;
        hash *= fnvPrime;
        data++;
    }
    return hash;
}

uint32_t rotate_left(uint32_t value, uint32_t count) {
    return value << count | value >> (32 - count);
}

uint32_t fxhash32_step(uint32_t hash, uint32_t value) {
    const uint32_t key = 0x27220a95;
    // const uint64_t key = 0x517cc1b727220a95;
    return (rotate_left(hash, 5) ^ value) * key;
}

uint32_t fxhash32_hash(uint8_t *data, int n) {
    uint32_t hash = 0;
    for (int i = n; i >= 4; i -= 4) {
        uint32_t number;
        memcpy(&number, data, sizeof(number));
        hash = fxhash32_step(hash, number);
        data += 4;
        n -= 4;
    }

    for (int i = 0; i < n; i++) {
        hash = fxhash32_step(hash, *data);
        data++;
    }

    return hash;
}

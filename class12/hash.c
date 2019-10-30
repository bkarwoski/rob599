#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#define TABLE_SIZE 8192
uint32_t table_hash_keys[256];

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

int main(int argc, char **argv) {
    char *hashFun = argv[1];
    uint8_t *input = (uint8_t *)argv[2];
    //printf("%d\n", strcmp(hashFun,"add"));
    if (strcmp(hashFun, "add") == 0) {
        printf("0x%x\n", add_hash(input, (int)strlen(argv[2])));
    } else if (strcmp(hashFun, "table_a") == 0) {
        setup_table_hash();
        printf("0x%x\n",table_a_hash(input, strlen(argv[2])));
    }
    return 0;
}


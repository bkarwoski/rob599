#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "hashtable.h"

int main(int argc, char* argv[]) {
    hashtable_t *ht = hashtable_create();
    char *key = "apple";
    int value = 100;
    hashtable_set(ht, key, value);
    value = 99;
    hashtable_get(ht, key, &value);
    printf("%d\n", value);
    hashtable_destroy(ht);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: ./golomb <max golomb number>\n");
        return 1;
    }
    int index = atoi(argv[1]);
    if (index == 0) {
        return 0;
    }
    vector_t g;
    vector_make(&g);
    vector_append(&g, 1);
    vector_append(&g, 2);
    vector_append(&g, 2);

    while (1) {
        if (size == capacity) {
            capacity *= 2;
            g = realloc(g,capacity);
        }

    }
    printf("%d\n",g[1]);

    free(g);
    return 0;
}
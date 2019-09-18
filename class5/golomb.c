#include <stdio.h>
#include <stdlib.h>

//reference used for making a dynamic vector array:
//https://eddmann.com/posts/implementing-a-dynamic-vector-array-in-c/

typedef struct vector {
    void **items;
    int capacity;
    int size;
} vector_t;

void vector_make(vector *v) {
    v->capacity = 32;
    v->size = 0;
    v->items = malloc(sizeof(void *) * v->capacity);
}

int vector_size(vector *v) {
    return v->size;
}

void vector_resize(vector *v, int capacity) {
    void **items = realloc(v->items, sizeof(void *) * capacity);
    if (items) {
        v->items = items;
        v->capacity = capacity;
    }
}

void vector_add(vector *v, int index, void *item) {
    if (v->capacity == v->size) {
        vector_resize(v, v->capacity * 2);
        v->items[v->size++] = item;

    }
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: ./golomb <max golomb number>\n");
        return 1;
    }
    int index = atoi(argv[1]);
    if (index == 0) {
        return 0;
    }
    int capacity = 32;
    int size = 3;
    int *g = malloc(sizeof(int) * capacity);

    g[0] = 1;
    g[1] = 2;
    g[2] = 2;

    while (1) {
        if (size == capacity) {
            capacity *= 2;
            g = realloc(g,capacity);
        }

    }
    //printf("%d\n",g[1]);

    free(g);
    return 0;
}
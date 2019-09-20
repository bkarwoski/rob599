#include <stdio.h>
#include <stdlib.h>

//reference used for making a dynamic vector array:
//https://eddmann.com/posts/implementing-a-dynamic-vector-array-in-c/

typedef struct vector {
    int *items;
    int capacity;
    int size;
} vector_t;

void vector_make(vector_t *v) {
    v->capacity = 32;
    v->size = 0;
    v->items = malloc(sizeof(int) * v->capacity);
}

void vector_resize(vector_t *v, int capacity) {
    void **items = realloc(v->items, sizeof(void *) * capacity);
    if (items) {
        v->items = items;
        v->capacity = capacity;
    }
}

void vector_append(vector_t *v, void *item) {
    if (v->capacity == v->size) {
        vector_resize(v, v->capacity * 2);
        v->items[v->size++] = item;

    }
}

void vector_free(vector_t *v) {
    free(v->items);
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
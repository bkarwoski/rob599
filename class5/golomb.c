#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct vector {
    size_t size;
    size_t capacity;
    int *data;
} vector_t;

vector_t vector_create(void) {
    vector_t v;
    v.size = 0;
    v.capacity = 4;
    v.data = malloc(sizeof(int) * v.capacity);
    return v;
}

void vector_append(vector_t *vec, int value) {
    if (vec->size >= vec->capacity) {
        //printf("calling realloc\n");
        vec->capacity *= 2;
        int *newData = realloc(vec->data, sizeof(int) * vec->capacity);
        vec->data = newData;
    }
    vec->data[vec->size] = value;
    vec->size++;
}

void vector_delete(vector_t *vec) {
    free(vec->data);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: ./golomb <max golomb number>\n");
        return 1;
    }
    int maxVal = atoi(argv[1]);
    if (maxVal == 0) {
        return 0;
    }
    if (maxVal == 1) {
        printf("1\n");
        return 0;
    }
    vector_t vec = vector_create();
    vector_append(&vec, 1);
    vector_append(&vec, 2);
    vector_append(&vec, 2);

    int addCount = 0;
    for (int i = 3; i <= maxVal; i++) {
        addCount = vec.data[i - 1];
        //printf("addCount = %d\n", addCount);
        for (int j = 0; j < addCount; j++) {
            vector_append(&vec, i);
        }
    }

    for (int i = vec.size - 1; i >= 0; i--) {
        printf("%d\n", vec.data[i]);
    }

    //printf("vec[3] = %d\n", vec.data[3]);
    vector_delete(&vec);
    return 0;
}

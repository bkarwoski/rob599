#include <stdlib.h>
#include "vector_xy_i32_t.h"
#include <stdint.h>

vector_xy_i32_t vector_i32_create(void) {
    vector_xy_i32_t v;
    v.size = 0;
    v.capacity = 8;
    v.xData = malloc(sizeof(int32_t) * v.capacity);
    v.yData = malloc(sizeof(int32_t) * v.capacity);
    return v;
}

void vector_i32_append(vector_xy_i32_t *vec, int32_t x, int32_t y) {
    if (vec->size >= vec->capacity) {
        vec->capacity *= 2;
        int32_t *newxData = realloc(vec->xData, sizeof(int32_t) * vec->capacity);
        int32_t *newyData = realloc(vec->yData, sizeof(int32_t) * vec->capacity);

        vec->xData = newxData;
        vec->yData = newyData;

    }
    vec->xData[vec->size] = x;
    vec->yData[vec->size] = y;
    vec->size++;
}

void vector_i32_delete(vector_xy_i32_t *vec) {
    free(vec->xData);
    free(vec->yData);

}
#pragma once
#include <stdlib.h>
#include <stdint.h>

typedef struct vector_xy_i32 {
    size_t size;
    size_t capacity;
    uint32_t *xData;
    uint32_t *yData;
} vector_xy_i32_t;

vector_xy_i32_t vector_i32_create(void);

void vector_i32_append(vector_xy_i32_t *vec, uint32_t x, uint32_t y);

void vector_i32_delete(vector_xy_i32_t *vec);
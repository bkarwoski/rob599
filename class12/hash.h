#pragma once

uint32_t add_hash(uint8_t *data, int n);

void setup_table_hash(void);

uint32_t table_a_hash(uint8_t *data, int n);

uint32_t table_b_hash(uint8_t *data, int n);

uint32_t djb2a_hash(uint8_t *data, int n);

uint32_t fnv1a_hash(uint8_t *data, int n);

uint32_t rotate_left(uint32_t value, uint32_t count);

uint32_t fxhash32_step(uint32_t hash, uint32_t value);

uint32_t fxhash32_hash(uint8_t *data, int n);

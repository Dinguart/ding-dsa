#ifndef DSUPERA_H
#define DSUPERA_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum boolean {
    TRUE=1, FALSE=0
} boolean;

typedef struct array {
    void *data;
    size_t size;
} array;

typedef struct vector {
  void *data;
  size_t size;         // stores the num of elements
  size_t max_limit;    // stores max capacity of vec
  size_t element_size; // size of element in bytes
} vector;

void init_vector(vector *vec, const size_t element_size);
vector init_vector_ret(const size_t element_size);

void vec_push_back(vector *vec, const void* item);
void* vec_front(const vector *vec);
void* vec_back(const vector *vec);
void* vec_item_at(const vector *vec, const size_t index);
int vec_idx_of_first(vector *vec, const void *item);
int vec_idx_of_last(vector *vec, const void *item);
array vec_all_occurrences_of(vector *vec, const void *item);
void vec_concat(vector *dest, const vector *src);

void vec_free(vector *vec);
void vec_reserve(vector *vec, const size_t new_max);

void info_log(const vector *v);

#endif

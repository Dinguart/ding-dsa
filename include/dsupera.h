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

// vector impl

typedef struct vector vector;
void init_vector(vector **vec, const size_t element_size);
vector *init_vector_ret(const size_t element_size);

void vec_push_back(vector *vec, const void *item, const size_t item_size);
void vec_pop_back(vector *vec);
void* vec_front(const vector *vec);
void* vec_back(const vector *vec);
void* vec_item_at(const vector *vec, const size_t index);
int vec_idx_of_first(vector *vec, const void *item, const size_t item_size);
int vec_idx_of_last(vector *vec, const void *item, const size_t item_size);
void vec_all_occurrences_of(array *output, vector *vec, const void *item, const size_t item_size);
void vec_concat(vector *dest, const vector *src);
void vec_clear(vector *vec);

int vec_is_empty(const vector *vec);
size_t vec_size(const vector *vec);
size_t vec_capacity(const vector *vec);

void vec_free(vector *vec);
void vec_reserve(vector *vec, const size_t new_max);
void vec_shrink_to_fit(vector *vec);

void vec_info_log(const vector *v);

// linked_list (doubly one) impl

typedef struct node node;
typedef struct linked_list linked_list;

node *init_node_ret(const void *data, const size_t element_size);
void init_node(node **n, const void *data, const size_t element_size);
void add_node(linked_list *ll, node *n);
void delete_node(node *n);

linked_list *init_linked_list_ret(const size_t element_size);
void init_linked_list(linked_list *ll, const size_t element_size);

void set_linked_list_head(linked_list *ll, const void *item, const size_t item_size);
void set_linked_list_tail(linked_list *ll, const void *item, const size_t item_size);

void free_node(node *n);
void free_linked_list(linked_list *ll);

void linked_list_info_log(linked_list *ll);

#endif

#ifndef DSUPERA_H
#define DSUPERA_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdalign.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

// for user-defined types & in order to test/display
typedef struct type_info {
    const char *type_name;
    size_t type_size;
    size_t alignment;
    void (*print)(const void *);
    size_t (*hash)(const void *); // for maps
    bool (*equal)(const void *, const void *);
} type_info;

// CONSIDER ____________________________________________________-
type_info *create_type_info_pair(const type_info *ti_first, const type_info *ti_second);

// primitives

extern const type_info TI_BOOL;
extern const type_info TI_CHAR;
extern const type_info TI_SCHAR;
extern const type_info TI_UCHAR;
extern const type_info TI_SHORT;
extern const type_info TI_USHORT;
extern const type_info TI_INT;
extern const type_info TI_UINT;
extern const type_info TI_LONG;
extern const type_info TI_ULONG;
extern const type_info TI_LONGLONG;
extern const type_info TI_ULONGLONG;
extern const type_info TI_FLOAT;
extern const type_info TI_DOUBLE;
extern const type_info TI_LONGDOUBLE;
extern const type_info TI_SIZE_T;
extern const type_info TI_STRING;

// to make a pair type info, must make a pair for each one (a lot of stuff to write lol)

// vector impl

typedef struct vector vector;
void init_vector(vector **vec, const type_info *ti);
vector *init_vector_ret(const type_info *ti);

void vec_push_back(vector *vec, const void *item, const type_info *ti);
void vec_pop_back(vector *vec);
void* vec_front(const vector *vec);
void* vec_back(const vector *vec);
void* vec_item_at(const vector *vec, const size_t index);
int vec_idx_of_first(vector *vec, const void *item, const type_info *ti);
int vec_idx_of_last(vector *vec, const void *item, const type_info *ti);
vector *vec_all_occurrences_of(vector *vec, const void *item, const type_info *ti);
void vec_concat(vector *dest, const vector *src);
void vec_clear(vector *vec);

bool vec_is_empty(const vector *vec);
size_t vec_size(const vector *vec);
size_t vec_capacity(const vector *vec);

void vec_free(vector *vec);
void vec_reserve(vector *vec, const size_t new_max);
void vec_shrink_to_fit(vector *vec);

void vec_info_log(const vector *v);

// linked_list (doubly one) impl

typedef struct node node;
typedef struct linked_list linked_list;

node *init_node_ret(const void *data, const type_info *ti);
void init_node(node **n, const void *data, const type_info *ti);
void node_push_front(linked_list *ll, node *n);
void node_push_back(linked_list *ll, node *n);
void delete_node(linked_list *ll, node *n);

linked_list *init_linked_list_ret(const type_info *ti);
void init_linked_list(linked_list *ll, const type_info *ti);
void *get_linked_list_head(const linked_list *ll);
void *get_linked_list_tail(const linked_list *ll);
size_t linked_list_size(linked_list *ll);



void free_node(node *n);
void free_linked_list(linked_list *ll);

void linked_list_info_log(linked_list *ll); // TODO: add more complex linked list functions

// queue (with doubly linked list implementation)

typedef struct queue queue;

queue *init_queue_ret(const type_info *ti);
void init_queue(queue **q, const type_info *ti);

void *queue_front(const queue *q);
void *queue_back(const queue *q);
void queue_push(queue *q, const void *data,
const type_info *ti);
void queue_pop(queue *q);

bool queue_is_empty(const queue *q);
size_t queue_size(const queue *q);

void queue_free(queue *q);

void queue_info_log(queue *q);

// pair
typedef struct pair pair;

pair *init_pair_ret(const void *left, const type_info *ti_left, const void *right, const type_info *ti_right);
void init_pair(pair **p, const void *left, const type_info *ti_left, const void *right, const type_info *ti_right);

void pair_free(pair *p);
void pair_info_log(pair *p);

// unordered map

typedef struct unorder_map unorder_map;

unorder_map *init_u_map_ret(const type_info *ti_key, const type_info *ti_val);
void init_u_map(unorder_map **um, const type_info *ti_key, const type_info *ti_val);

#endif

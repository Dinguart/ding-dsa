#include "dsupera.h"

// primitives for ease of use

static void print_bool(const void *item) { printf("%s\n", *(bool*)item?"true":"false"); }
static void print_char(const void *item) { printf("%c\n", *(char*)item); }
static void print_schar(const void *item) { printf("%hhi\n", *(signed char*)item); }
static void print_uchar(const void *item) { printf("%hhu\n", *(unsigned char*)item); }
static void print_short(const void *item) { printf("%hi\n", *(short*)item); }
static void print_ushort(const void *item) { printf("%hu\n", *(unsigned short*)item); }
static void print_int(const void *item) { printf("%i\n", *(int*)item); }
static void print_uint(const void *item) { printf("%u\n", *(unsigned*)item); }
static void print_long(const void *item) { printf("%li\n", *(long*)item); }
static void print_ulong(const void *item) { printf("%lu\n", *(unsigned long*)item); }
static void print_longlong(const void *item) { printf("%lli\n", *(long long*)item); }
static void print_ulonglong(const void *item) { printf("%llu\n", *(unsigned long long*)item); }
static void print_float(const void *item) { printf("%f\n", *(float*)item); }
static void print_double(const void *item) { printf("%lf\n", *(double*)item); }
static void print_longdouble(const void *item) { printf("%Lf\n", *(long double*)item); }
static void print_size_t(const void *item) { printf("%zu\n", *(size_t*)item); }

const type_info TI_BOOL = { "bool", sizeof(bool), print_bool };
const type_info TI_CHAR = { "char", sizeof(char), print_char };
const type_info TI_SCHAR = { "signed char", sizeof(signed char), print_schar };
const type_info TI_UCHAR = { "unsigned char", sizeof(unsigned char), print_uchar };
const type_info TI_SHORT = { "short", sizeof(short), print_short };
const type_info TI_USHORT = { "unsigned short", sizeof(unsigned short), print_ushort };
const type_info TI_INT = { "int", sizeof(int), print_int };
const type_info TI_UINT = { "unsigned int", sizeof(unsigned), print_uint };
const type_info TI_LONG = { "long", sizeof(long), print_long };
const type_info TI_ULONG = { "unsigned long", sizeof(unsigned long), print_ulong };
const type_info TI_LONGLONG = { "long long", sizeof(long long), print_longlong };
const type_info TI_ULONGLONG = { "unsigned long long", sizeof(unsigned long long), print_ulonglong };
const type_info TI_FLOAT = { "float", sizeof(float), print_float };
const type_info TI_DOUBLE = { "double", sizeof(double), print_double };
const type_info TI_LONGDOUBLE = { "long double", sizeof(long double), print_longdouble };
const type_info TI_SIZE_T = { "size_t", sizeof(size_t), print_size_t };

// regular lib impl

typedef struct object {
    const type_info *ti;
    void *data;
    size_t element_size;
} object;

// vector implementation

typedef struct vector {
    object obj;
    size_t max_limit;    // stores max capacity of vec
    size_t size; // size of element in bytes
} vector;

void init_vector(vector **vec, const type_info *ti) {
    if (!ti) {
        fprintf(stderr, "Must specify type info to use vector.\n");
        return;
    }
    if (!*vec) {
        *vec = malloc(sizeof(vector));
    }

    (*vec)->size = 0;
    (*vec)->max_limit = 4;
    (*vec)->obj.element_size = ti->type_size;
    (*vec)->obj.data = malloc(ti->type_size * (*vec)->max_limit);
    (*vec)->obj.ti = ti;
}

vector *init_vector_ret(const type_info *ti) {
    if (!ti) {
        fprintf(stderr, "Must specify type info to use vector.\n");
        return NULL;
    }

    size_t max_limit = 4;
    vector *v = malloc(sizeof(vector));
    v->obj.data = malloc(ti->type_size*max_limit);
    v->size = 0;
    v->max_limit = max_limit;
    v->obj.element_size = ti->type_size;
    v->obj.ti = ti;
    return v;
}

void vec_push_back(vector *vec, const void *item, const type_info *ti) {
    if (!ti || ti != vec->obj.ti) {
        fprintf(stderr, "Type mismatch between the item and the vector.\n");
        return;
    }
    if (vec->size+1 > vec->max_limit) {
        vec->max_limit *= 2;
        vec->obj.data = realloc(vec->obj.data, vec->max_limit * vec->obj.element_size);
    }

    void *dest = (char *)vec->obj.data + (vec->size * vec->obj.element_size);
    memcpy(dest, item, vec->obj.element_size);
    vec->size++;
}

void vec_pop_back(vector *vec) {
    if (vec->size == 0) {
        fprintf(stderr, "Cannot pop back, vector is empty.\n");
        return;
    }
    vec->size--;
}

void *vec_front(const vector *vec) {
    if (vec->size == 0) {
        fprintf(stderr, "Vector has no elements.\n");
        return NULL;
    }
    return vec->obj.data;
}

void *vec_back(const vector *vec) {
    if (vec->size == 0) {
        fprintf(stderr, "Vector has no elements.\n");
        return NULL;
    }
    return (char*)vec->obj.data + (vec->size-1)*vec->obj.element_size;
}

void *vec_item_at(const vector *vec, const size_t index) {
    return (char*)vec->obj.data + index*vec->obj.element_size;
}

void vec_reserve(vector *vec, const size_t new_max) {
    if (new_max < vec->size) {
        fprintf(stderr, "Cannot reserve smaller than vector's size\n");
        return;
    }
    vec->max_limit = new_max;
    vec->obj.data = realloc(vec->obj.data, vec->max_limit * vec->obj.element_size);
}

int vec_idx_of_first(vector *vec, const void *item, const type_info *ti) {
    if (!ti || ti != vec->obj.ti) {
        fprintf(stderr, "Type mismatch between the item and the vector.\n");
        return -1;
    }

    for (size_t i=0; i<vec->size; ++i) {
        if (memcmp((char*)vec->obj.data+i*vec->obj.element_size, item, vec->obj.element_size)==0) return i;
    }
    return -1;
}

int vec_idx_of_last(vector *vec, const void *item, const type_info *ti) {
    if (!ti || ti != vec->obj.ti) {
        fprintf(stderr, "Type mismatch between the item and the vector.\n");
        return -1;
    }

    for (int i=(int)vec->size; i>=0; --i) {
        if (memcmp((char*)vec->obj.data+i*vec->obj.element_size, item, vec->obj.element_size)==0) return i;
    }
    return -1;
}

vector *vec_all_occurrences_of(vector *vec, const void *item, const type_info *ti) {
    if (!ti || ti != vec->obj.ti) {
        fprintf(stderr, "Type mismatch between the item and the vector.\n");
        return NULL;
    }

    vector *output = init_vector_ret(&TI_SIZE_T);
    for (size_t i=0; i<vec->size; ++i) {
        if (memcmp((char*)vec->obj.data+i*vec->obj.element_size, item, vec->obj.element_size)==0) {
            vec_push_back(output, &i, &TI_SIZE_T);
        }
    }
    vec_shrink_to_fit(output);
    return output;
}

void vec_concat(vector *dest, const vector *src) {
    if (dest->obj.ti != src->obj.ti) {
        fprintf(stderr, "Vectors contain different types.\n");
        return;
    }

    if (dest->size+src->size > dest->max_limit) {
        dest->max_limit = dest->max_limit + src->max_limit;
        dest->obj.data = realloc(dest->obj.data, dest->max_limit * dest->obj.element_size);
    }

    size_t src_idx=0;
    for (size_t i = dest->size; i<dest->size+src->size; ++i) {
        void *item = (char *)src->obj.data + (src_idx++ * src->obj.element_size);
        void *dest_tmp = (char *)dest->obj.data + (i * dest->obj.element_size);
        memcpy(dest_tmp, item, dest->obj.element_size);
    }
    dest->size += src->size;
}

void vec_clear(vector *vec) {
    memset(vec->obj.data, 0, vec->size * vec->obj.element_size);
    vec->size = 0;
}

bool vec_is_empty(const vector *vec) {
    return vec->size == 0;
}
size_t vec_size(const vector *vec) {
    return vec->size;
}
size_t vec_capacity(const vector *vec) {
    return vec->max_limit;
}

void vec_free(vector *vec) {
    free(vec->obj.data);
    vec->size=0;
    vec->max_limit=0;
    vec->obj.element_size=0;
}

void vec_shrink_to_fit(vector *vec) {
    vec->max_limit = vec->size;
    vec->obj.data = realloc(vec->obj.data, vec->max_limit * vec->obj.element_size);
}

// replace with future print functionality
void vec_info_log(const vector *v) {
    if (!v) {
        fprintf(stderr, "Vector is null.\n");
        return;
    }
    printf("vector size -> %zu,\nvector max limit -> %zu,\nvector element size -> %lu,\n", v->size, v->max_limit, v->obj.element_size);
    if (v->size == 0) return;

    for (size_t i = 0; i < v->size; ++i) {
        v->obj.ti->print((char*)v->obj.data + (v->obj.element_size * i));
    }
}


// linked list implementation

typedef struct node {
    object obj;
    node *next;
    node *prev;
} node;

typedef struct linked_list {
    node *head;
    node *tail;
    size_t size;
    size_t element_size;
} linked_list;

node *init_node_ret(const void *data, const size_t element_size) {
    node *n = malloc(sizeof(node));
    n->obj.data = malloc(element_size);
    n->obj.element_size = element_size;
    n->next = NULL;
    n->prev = NULL;
    memcpy(n->obj.data, data, element_size);
    return n;
}
void init_node(node **n, const void *data, const size_t element_size) {
    if (!*n) {
        *n = malloc(sizeof(node));
    }

    (*n)->obj.data = malloc(element_size);
    (*n)->obj.element_size = element_size;
    (*n)->next = NULL;
    (*n)->prev = NULL;
    memcpy((*n)->obj.data, data, element_size);
}

void node_push_front(linked_list *ll, node *n) {
    if (ll->element_size != n->obj.element_size) {
        fprintf(stderr, "Node is of different type.\n");
        return;
    }
    if (!n) {
        fprintf(stderr, "Node must be initialized.\n");
        return;
    }

    node *tmp = ll->head->next;
    n->next = tmp;
    n->prev = ll->head;
    ll->head->next = n;
    tmp->prev = n;

    ll->size++;
}

void node_push_back(linked_list *ll, node *n) {
    if (ll->element_size != n->obj.element_size) {
        fprintf(stderr, "Node is of different type.\n");
        return;
    }
    if (!n) {
        fprintf(stderr, "Node must be initialized.\n");
        return;
    }

    node *tmp = ll->tail->prev;
    n->next = ll->tail;
    n->prev = tmp;
    ll->tail->prev = n;
    tmp->next = n;

    ll->size++;
}

void delete_node(linked_list *ll, node *n) {
    if (ll->size == 0) {
        fprintf(stderr, "Linked list is empty, cannot pop.\n");
        return;
    }

    node *prev_n = n->prev;
    node *next_n = n->next;
    prev_n->next = next_n;
    next_n->prev = prev_n;
    free_node(n);

    ll->size--;
}

linked_list *init_linked_list_ret(const size_t element_size) {
    linked_list *ll = malloc(sizeof(linked_list));
    ll->element_size = element_size;

    ll->head = malloc(sizeof(node));
    ll->tail = malloc(sizeof(node));

    ll->head->obj.data = malloc(element_size);
    ll->tail->obj.data = malloc(element_size);

    ll->head->next = ll->tail;
    ll->tail->prev = ll->head;

    ll->size = 0;
    return ll;
}

void init_linked_list(linked_list *ll, const size_t element_size) {
    ll->element_size = element_size;

    ll->head = malloc(sizeof(node));
    ll->tail = malloc(sizeof(node));

    ll->head->obj.data = malloc(element_size);
    ll->tail->obj.data = malloc(element_size);

    ll->head->next = ll->tail;
    ll->tail->prev = ll->head;

    ll->size = 0;
}

void *get_linked_list_head(const linked_list *ll) {
    return ll->tail->prev;
}

void *get_linked_list_tail(const linked_list *ll) {
    return ll->head->next;
}

size_t linked_list_size(linked_list *ll) {
    return ll->size;
}

void free_node(node *n) {
    free(n->obj.data);
    free(n);
}

void free_linked_list(linked_list *ll) {
    node *tmp = ll->head;
    while (tmp) {
        tmp = ll->head->next;
        free_node(ll->head);
    }
    free(ll);
}

void linked_list_info_log(linked_list *ll) {
    node *tmp = ll->head->next;
    size_t counter=0;
    while (tmp) {
        if (counter++ == ll->size) break;
        printf("%d\n", *(int*)(tmp->obj.data));
        tmp = tmp->next;
    }
    printf("size: %zu\n", ll->size);
}

// queue implementation

typedef struct queue {
    linked_list *queue_data;
} queue;

queue *init_queue_ret(const size_t element_size) {
    queue *q = malloc(sizeof(queue));
    q->queue_data = init_linked_list_ret(element_size);
    return q;
}

void init_queue(queue **q, const size_t element_size) {
    if (!*q) *q = malloc(sizeof(queue));
    (*q)->queue_data = init_linked_list_ret(element_size);
}

void *queue_front(const queue *q) {
    return get_linked_list_head(q->queue_data);
}

void *queue_back(const queue *q) {
    return get_linked_list_tail(q->queue_data);
}

void queue_push(queue *q, const void *data, const size_t element_size) {
    node_push_back(q->queue_data, init_node_ret(data, element_size));
}

void queue_pop(queue *q) {
    delete_node(q->queue_data, q->queue_data->tail->prev);
}

bool queue_is_empty(const queue *q) {
    return q->queue_data->size == 0;
}

size_t queue_size(const queue *q) {
    return q->queue_data->size;
}

void queue_free(queue *q) {
    free_linked_list(q->queue_data);
}

void queue_info_log(queue *q) {
    linked_list_info_log(q->queue_data);
}

// pair impl

typedef struct pair {
    object left;
    object right;
} pair;

pair *init_pair_ret(const void *left, const size_t left_elem_size, const void *right, const size_t right_elem_size) {
    pair *p = malloc(sizeof(pair));

    p->left.data = malloc(left_elem_size);
    p->right.data = malloc(right_elem_size);

    p->left.element_size = left_elem_size;
    p->right.element_size = right_elem_size;

    memcpy(p->left.data, left, left_elem_size);
    memcpy(p->right.data, right, right_elem_size);
    return p;
}

void init_pair(pair **p, const void *left, const size_t left_elem_size, const void *right, const size_t right_elem_size) {
    if (!*p) *p = malloc(sizeof(pair));

    (*p)->left.data = malloc(left_elem_size);
    (*p)->right.data = malloc(right_elem_size);
    (*p)->left.element_size = left_elem_size;
    (*p)->right.element_size = right_elem_size;
    memcpy((*p)->left.data, left, left_elem_size);
    memcpy((*p)->right.data, right, right_elem_size);
}

void pair_free(pair *p) {
    free(p->left.data);
    free(p->right.data);
    free(p);
}

void pair_info_log(pair *p) {
    printf("%d %s\n", *(int*)p->left.data, (char*)p->right.data);
}

// unordered map impl

typedef struct unorder_map {
    linked_list *buckets;

    size_t bucket_size;
    size_t num_kv_pairs;
    float max_load_f;
} unorder_map;

// implement linked list bucket system with hashing function.

unorder_map *init_u_map_ret(const size_t key_elem_size, const size_t val_elem_size) {

}

void init_u_map(unorder_map *um, const size_t key_elem_size, const size_t val_elem_size) {

}

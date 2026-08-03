#include "dsupera.h"

// structs

typedef struct object {
    const type_info *ti;
    void *data;
    size_t element_size;
} object;

typedef struct vector {
    object obj;
    size_t max_limit;    // stores max capacity of vec
    size_t size; // size of element in bytes
} vector;

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

typedef struct queue {
    linked_list *queue_data;
} queue;

typedef struct pair {
    object left;
    object right;
} pair;

typedef struct unorder_map {
    vector *buckets;
    size_t num_kv_pairs;
    float max_load_f;
} unorder_map;

// some helpers

static bool is_prime(int n) {
    if (n == 2 || n == 3) return true;
    int s = (int)sqrt(n);
    for (int i=1; i<s; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}


// primitive type info functions for ease of use

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
static void print_string(const void *item) { printf("%s\n", *(char**)item); }

static void print_type_info_pair(const void *item) {
    const pair *p = (const pair*)item;
    p->left.ti->print(p->left.data);
    p->right.ti->print(p->right.data);
}

// utility (for creating type info pairs.)

type_info *create_type_info_pair(const type_info *ti_first, const type_info *ti_second) {
    type_info *ti_pair = malloc(sizeof(type_info));

    size_t name_size = strlen(ti_first->type_name) + strlen(ti_second->type_name);
    char *pair_name = malloc(6 + name_size);
    strcat(pair_name, "pair ");
    strcat(pair_name, ti_first->type_name);
    strcat(pair_name, ti_second->type_name);
    pair_name[5 + name_size] = '\0';
    ti_pair->type_name = pair_name;

    size_t pair_alignment = (size_t)max(ti_first->type_size, ti_second->type_size) == ti_first->type_size ? ti_first->alignment : ti_second->alignment;

    ti_pair->type_size = max(ti_first->type_size, ti_second->type_size) + pair_alignment;
    ti_pair->alignment = pair_alignment;

    ti_pair->print = print_type_info_pair;
    return ti_pair;
}

// hash functions

static size_t fmix32(const void *item) {
    size_t h = *(size_t*)item;
    h ^= h >> 16;
	h *= 0x85ebca6bu;
	h ^= h >> 13;
	h *= 0xc2b2ae35u;
	h ^= h >> 16;

	return h;
}

static unsigned long fmix64(const void *item) {
    unsigned long h = *(unsigned long*)item;
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccd;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53;
    h ^= h >> 33;
    return h;
}

static size_t float_hash(const void *item) {
    float n_f = (*(float*)item) != 0 ? (*(float*)item) : 0.0;
    unsigned long b;
    memcpy(&b, &n_f, sizeof(b));
    return fmix32(&b);
}

static unsigned long str_hash(const void *item) {
    unsigned long h = 0x1505;
    int c;
    const unsigned char *s = *(const unsigned char**)item;
    while ((c = *s++)) {
        h = ((h << 5) + h) + c;
    }
    return h;
}

// equality functions

static bool equal_float(const void *f1, const void *f2) {
    unsigned long b1, b2;
    float f1_f = *(float*)f1;
    float f2_f = *(float*)f2;
    memcpy(&b1, &f1_f, sizeof(b1));
    memcpy(&b2, &f2_f, sizeof(b2));
    return f1_f == f2_f || b1 == b2;
}

// default lib type info objects.

const type_info TI_BOOL = { "bool", sizeof(bool), alignof(bool), print_bool };
const type_info TI_CHAR = { "char", sizeof(char), alignof(char), print_char };
const type_info TI_SCHAR = { "signed char", sizeof(signed char), alignof(signed char), print_schar };
const type_info TI_UCHAR = { "unsigned char", sizeof(unsigned char), alignof(unsigned char), print_uchar };
const type_info TI_SHORT = { "short", sizeof(short), alignof(short), print_short };
const type_info TI_USHORT = { "unsigned short", sizeof(unsigned short), alignof(unsigned short), print_ushort };
const type_info TI_INT = { "int", sizeof(int), alignof(int), print_int };
const type_info TI_UINT = { "unsigned int", sizeof(unsigned), alignof(unsigned), print_uint };
const type_info TI_LONG = { "long", sizeof(long), alignof(long), print_long };
const type_info TI_ULONG = { "unsigned long", sizeof(unsigned long), alignof(unsigned long), print_ulong };
const type_info TI_LONGLONG = { "long long", sizeof(long long), alignof(long long), print_longlong };
const type_info TI_ULONGLONG = { "unsigned long long", sizeof(unsigned long long), alignof(unsigned long long), print_ulonglong };
const type_info TI_FLOAT = { "float", sizeof(float), alignof(float), print_float };
const type_info TI_DOUBLE = { "double", sizeof(double), alignof(double), print_double };
const type_info TI_LONGDOUBLE = { "long double", sizeof(long double), alignof(long double), print_longdouble };
const type_info TI_SIZE_T = { "size_t", sizeof(size_t), alignof(size_t), print_size_t };
const type_info TI_STRING = { "string", sizeof(char*), alignof(char*), print_string };

// vector implementation
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

void vec_insert(vector *vec, const void *item, const type_info *ti, size_t idx) {
        if (!ti || ti != vec->obj.ti) {
        fprintf(stderr, "Type mismatch between the item and the vector.\n");
        return;
    }
    if (vec->size+1 > vec->max_limit) {
        vec->max_limit *= 2;
        vec->obj.data = realloc(vec->obj.data, vec->max_limit * vec->obj.element_size);
    }
    if (idx >= vec->max_limit) {
        fprintf(stderr, "Index exceeds vector size.\n");
        return;
    }
    void *dest = (char *)vec->obj.data + (idx * vec->obj.element_size);
    memcpy(dest, item, vec->obj.element_size);
    vec->size++;
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
node *init_node_ret(const void *data, const type_info *ti) {
    if (!ti) {
        fprintf(stderr, "Must set type info to create a node.\n");
        return NULL;
    }

    node *n = malloc(sizeof(node));
    n->obj.data = malloc(ti->type_size);
    n->obj.element_size = ti->type_size;
    n->obj.ti = ti;
    n->next = NULL;
    n->prev = NULL;
    memcpy(n->obj.data, data, ti->type_size);
    return n;
}
void init_node(node **n, const void *data, const type_info *ti) {
    if (!ti) {
        fprintf(stderr, "Must set type info to create a node.\n");
        return;
    }
    if (!*n) {
        *n = malloc(sizeof(node));
    }

    (*n)->obj.data = malloc(ti->type_size);
    (*n)->obj.element_size = ti->type_size;
    (*n)->obj.ti = ti;
    (*n)->next = NULL;
    (*n)->prev = NULL;
    memcpy((*n)->obj.data, data, ti->type_size);
}

void node_push_front(linked_list *ll, node *n) {
    if (ll->head->obj.ti != n->obj.ti) {
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
    if (ll->head->obj.ti != n->obj.ti) {
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

linked_list *init_linked_list_ret(const type_info *ti) {
    if (!ti) {
        fprintf(stderr, "Type info must be set for the linked list.\n");
        return NULL;
    }
    linked_list *ll = malloc(sizeof(linked_list));
    ll->element_size = ti->type_size;

    ll->head = malloc(sizeof(node));
    ll->tail = malloc(sizeof(node));

    ll->head->obj.data = malloc(ti->type_size);
    ll->tail->obj.data = malloc(ti->type_size);
    ll->head->obj.element_size = ti->type_size;
    ll->tail->obj.element_size = ti->type_size;
    ll->head->obj.ti = ti;
    ll->tail->obj.ti = ti;

    ll->head->next = ll->tail;
    ll->tail->prev = ll->head;

    ll->size = 0;
    return ll;
}

void init_linked_list(linked_list *ll, const type_info *ti) {
    if (!ti) {
        fprintf(stderr, "Type info must be set for the linked list.\n");
        return;
    }
    ll->element_size = ti->type_size;

    ll->head = malloc(sizeof(node));
    ll->tail = malloc(sizeof(node));

    ll->head->obj.data = malloc(ti->type_size);
    ll->tail->obj.data = malloc(ti->type_size);
    ll->head->obj.element_size = ti->type_size;
    ll->tail->obj.element_size = ti->type_size;
    ll->head->obj.ti = ti;
    ll->tail->obj.ti = ti;

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
        tmp->obj.ti->print(tmp->obj.data);
        tmp = tmp->next;
    }
    printf("size: %zu\n", ll->size);
}

static void linked_list_print(const void *item) {
    const linked_list *ll = (linked_list*)item;
    node *tmp = ll->head->next;
    size_t ctr=0;
    while (tmp) {
        if (ctr++ == ll->size) break;
        tmp->obj.ti->print(tmp->obj.data);
        tmp = tmp->next;
    }
    printf("size: %zu\n", ll->size);
}

// queue implementation
queue *init_queue_ret(const type_info *ti) {
    queue *q = malloc(sizeof(queue));
    q->queue_data = init_linked_list_ret(ti);
    return q;
}

void init_queue(queue **q, const type_info *ti) {
    if (!*q) *q = malloc(sizeof(queue));
    (*q)->queue_data = init_linked_list_ret(ti);
}

void *queue_front(const queue *q) {
    return get_linked_list_head(q->queue_data);
}

void *queue_back(const queue *q) {
    return get_linked_list_tail(q->queue_data);
}

void queue_push(queue *q, const void *data, const type_info *ti) {
    node_push_back(q->queue_data, init_node_ret(data, ti));
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
pair *init_pair_ret(const void *left, const type_info *ti_left, const void *right, const type_info *ti_right) {
    if (!ti_left || !ti_right) {
        fprintf(stderr, "Must create type info for a pair.\n");
        return NULL;
    }
    pair *p = malloc(sizeof(pair));

    p->left.data = malloc(ti_left->type_size);
    p->right.data = malloc(ti_right->type_size);

    p->left.element_size = ti_left->type_size;
    p->right.element_size = ti_right->type_size;

    p->left.ti = ti_left;
    p->right.ti = ti_right;

    memcpy(p->left.data, left, ti_left->type_size);
    memcpy(p->right.data, right, ti_right->type_size);
    return p;
}

void init_pair(pair **p, const void *left, const type_info *ti_left, const void *right, const type_info *ti_right) {
    if (!ti_left || !ti_right) {
        fprintf(stderr, "Must create type info for a pair.\n");
        return;
    }
    if (!*p) *p = malloc(sizeof(pair));

    (*p)->left.data = malloc(ti_left->type_size);
    (*p)->right.data = malloc(ti_right->type_size);

    (*p)->left.element_size = ti_left->type_size;
    (*p)->right.element_size = ti_right->type_size;

    (*p)->left.ti = ti_left;
    (*p)->right.ti = ti_right;

    memcpy((*p)->left.data, left, ti_left->type_size);
    memcpy((*p)->right.data, right, ti_right->type_size);
}

void pair_free(pair *p) {
    free(p->left.data);
    free(p->right.data);
    free(p);
}

void pair_info_log(pair *p) {
    p->left.ti->print(p->left.data);
    p->right.ti->print(p->right.data);
}

// unordered map impl
// implement linked list bucket system with hashing function.


unorder_map *init_u_map_ret(const type_info *ti_key, const type_info *ti_val) {
    unorder_map *um = malloc(sizeof(unorder_map));
    pair *p = malloc(sizeof(pair));
    p->left.data = malloc(ti_key->type_size);
    p->right.data = malloc(ti_val->type_size);

    p->left.element_size = ti_key->type_size;
    p->right.element_size = ti_val->type_size;

    p->left.ti = ti_key;
    p->right.ti = ti_val;

    const type_info *pair_info = create_type_info_pair(ti_key, ti_val);

    linked_list *l = init_linked_list_ret(pair_info);

    const type_info ll_info = {
        .type_name = "linked list pair",
        .type_size = sizeof(*l),
        .alignment = alignof(*l),
        .print = linked_list_print
        // todo: add a static equal method.
    };

    um->buckets = init_vector_ret(&ll_info);
    vec_reserve(um->buckets, 1);
    um->num_kv_pairs = 0;
    um->max_load_f = 1.0f;
}

void init_u_map(unorder_map **um, const type_info *ti_key, const type_info *ti_val) {
    if (!ti_key || !ti_val) {
        fprintf(stderr, "Must create type info for a key value pair.\n");
        return;
    }
    if (!*um) *um = malloc(sizeof(unorder_map));

    pair *p = malloc(sizeof(pair));
    p->left.data = malloc(ti_key->type_size);
    p->right.data = malloc(ti_val->type_size);

    p->left.element_size = ti_key->type_size;
    p->right.element_size = ti_val->type_size;

    p->left.ti = ti_key;
    p->right.ti = ti_val;

    const type_info *pair_info = create_type_info_pair(ti_key, ti_val);

    linked_list *l = init_linked_list_ret(pair_info);

    const type_info ll_info = {
        .type_name = "linked list pair",
        .type_size = sizeof(*l),
        .alignment = alignof(*l),
        .print = linked_list_print
        // todo: add a static equal method.
    };

    (*um)->buckets = init_vector_ret(&ll_info);
    vec_reserve((*um)->buckets, 1);
    (*um)->num_kv_pairs = 0;
    (*um)->max_load_f = 1.0f;
}

void umap_rehash(unorder_map *um) {
    if (um->buckets->size / um->num_kv_pairs <= um->max_load_f) return;

    int i = (int)um->buckets->size;
    while (!is_prime(i)) {
        ++i;
    }
    vector *new_buckets = init_vector_ret(um->buckets->obj.ti);
    vec_reserve(new_buckets, (size_t)i);
    
    for (size_t i = 0; i < v->size; ++i) {
        linked_list *ll = (linked_list*)(um->buckets->obj.data + (um->buckets->obj.element_size * i));
        node *tmp = ll->head->next;
        size_t counter=0;
        while (tmp) {
            if (counter++ == ll->size) break;
            unsigned long hash = tmp->obj.ti->hash(tmp->obj.data);
            tmp = tmp->next;
            size_t idx = hash % um->buckets->size;
            vec_insert(new_buckets, ll, ll->head->obj.ti, idx);
        }
    }
    vec_clear(um->buckets);
    um->buckets = new_buckets;
}


void umap_hash(unorder_map *um, void *key) {
    int hashed_key = um->
}

void umap_insert(unorder_map *um, const void *key, const type_info *ti_key, const void *val, const type_info *ti_val) {
    if (!ti_key || !ti_val) {
        fprintf(stderr, "Must initialize type info structs.\n");
        return;
    }
    
    pair *p = init_pair_ret(key, ti_key, val, ti_val);
    const type_info *pi = create_type_info_pair(ti_key, ti_val);
    //if (pi != um->
    
    node *pair_node = init_node_ret(p, pi);
    
}

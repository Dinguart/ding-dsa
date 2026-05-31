#include "dsupera.h"

// vector implementation

typedef struct vector {
    void *data;
    size_t size;         // stores the num of elements
    size_t max_limit;    // stores max capacity of vec
    size_t element_size; // size of element in bytes
} vector;

void init_vector(vector **vec, const size_t element_size) {
    if (!*vec) {
        *vec = malloc(sizeof(vector));
    }

    (*vec)->size = 0;
    (*vec)->max_limit = 4;
    (*vec)->element_size = element_size;
    (*vec)->data = malloc(element_size * (*vec)->max_limit);
}

vector *init_vector_ret(const size_t element_size) {
    size_t max_limit = 4;
    vector *v = malloc(sizeof(vector));
    v->data = malloc(element_size*max_limit);
    v->size = 0;
    v->max_limit = max_limit;
    v->element_size = element_size;
    return v;
}

void vec_push_back(vector *vec, const void *item, const size_t item_size) {
    if (item_size != vec->element_size) {
        fprintf(stderr, "Type mismatch between the item and the vector.\n");
        return;
    }
    if (vec->size+1 > vec->max_limit) {
        vec->max_limit *= 2;
        vec->data = realloc(vec->data, vec->max_limit * vec->element_size);
    }

    void *dest = (char *)vec->data + (vec->size * vec->element_size);
    memcpy(dest, item, vec->element_size);
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
    return vec->data;
}

void *vec_back(const vector *vec) {
    if (vec->size == 0) {
        fprintf(stderr, "Vector has no elements.\n");
        return NULL;
    }
    return (char*)vec->data + (vec->size-1)*vec->element_size;
}

void *vec_item_at(const vector *vec, const size_t index) {
    return (char*)vec->data + index*vec->element_size;
}

void vec_reserve(vector *vec, const size_t new_max) {
    if (new_max < vec->size) {
        fprintf(stderr, "Cannot reserve smaller than vector's size\n");
        return;
    }
    vec->max_limit = new_max;
    vec->data = realloc(vec->data, vec->max_limit * vec->element_size);
}

int vec_idx_of_first(vector *vec, const void *item, const size_t item_size) {
    if (item_size != vec->element_size) {
        fprintf(stderr, "Type mismatch between the item and the vector.\n");
        return -1;
    }

    for (size_t i=0; i<vec->size; ++i) {
        if (memcmp((char*)vec->data+i*vec->element_size, item, vec->element_size)==0) return i;
    }
    return -1;
}

int vec_idx_of_last(vector *vec, const void *item, const size_t item_size) {
    if (item_size != vec->element_size) {
        fprintf(stderr, "Type mismatch between the item and the vector.\n");
        return -1;
    }

    for (int i=(int)vec->size; i>=0; --i) {
        if (memcmp((char*)vec->data+i*vec->element_size, item, vec->element_size)==0) return i;
    }
    return -1;
}

void vec_all_occurrences_of(array *output, vector *vec, const void *item, const size_t item_size) {
    if (item_size != vec->element_size) {
        fprintf(stderr, "Type mismatch between the item and the vector.\n");
        return;
    }

    output->data = (int*)malloc(sizeof(int) * vec->size);
    output->size = vec->size;

    size_t idx=0;
    for (size_t i=0; i<vec->size; ++i) {
        if (memcmp((char*)vec->data+i*vec->element_size, item, vec->element_size)==0) {
            ((int*)output->data)[idx++] = i;
        }
    }
    if (idx < vec->size) {
        output->data = realloc(output->data, sizeof(int)*idx);
        output->size = idx;
    }
}

void vec_concat(vector *dest, const vector *src) {
    if (dest->element_size != src->element_size) {
        fprintf(stderr, "Vectors contain different types.\n");
        return;
    }

    if (dest->size+src->size > dest->max_limit) {
        dest->max_limit = dest->max_limit + src->max_limit;
        dest->data = realloc(dest->data, dest->max_limit * dest->element_size);
    }

    size_t src_idx=0;
    for (size_t i = dest->size; i<dest->size+src->size; ++i) {
        void *item = (char *)src->data + (src_idx++ * src->element_size);
        void *dest_tmp = (char *)dest->data + (i * dest->element_size);
        memcpy(dest_tmp, item, dest->element_size);
    }
    dest->size += src->size;
}

void vec_clear(vector *vec) {
    memset(vec->data, 0, vec->size * vec->element_size);
    vec->size = 0;
}

int vec_is_empty(const vector *vec) {
    return vec->size == 0;
}
size_t vec_size(const vector *vec) {
    return vec->size;
}
size_t vec_capacity(const vector *vec) {
    return vec->max_limit;
}

void vec_free(vector *vec) {
    free(vec->data);
    vec->size=0;
    vec->max_limit=0;
    vec->element_size=0;
}

void vec_shrink_to_fit(vector *vec) {
    vec->max_limit = vec->size;
    vec->data = realloc(vec->data, vec->max_limit * vec->element_size);
}

void vec_info_log(const vector *v) {
    printf("vector size -> %zu,\nvector max limit -> %zu,\nvector element size -> %lu,\n", v->size, v->max_limit, v->element_size);
    if (v->size == 0) return;
    for (size_t i = 0; i < v->size; ++i) {
        printf("%zu -> %d\n", i, ((int*)v->data)[i]);
    }
}


// linked list implementation

typedef struct node {
    void *data;
    size_t element_size;
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
    n->data = malloc(element_size);
    n->element_size = element_size;
    n->next = NULL;
    n->prev = NULL;
    memcpy(n->data, data, element_size);
    return n;
}
void init_node(node **n, const void *data, const size_t element_size) {
    if (!*n) {
        *n = malloc(sizeof(node));
    }

    (*n)->data = malloc(element_size);
    (*n)->element_size = element_size;
    (*n)->next = NULL;
    (*n)->prev = NULL;
    memcpy((*n)->data, data, element_size);
}

void add_node(linked_list *ll, node *n) {
    if (ll->element_size != n->element_size) {
        fprintf(stderr, "Node is of different type.\n");
        return;
    }

    node *tmp = ll->head->next;
    n->next = tmp;
    n->prev = ll->head;
    ll->head->next = n;
    tmp->prev = n;
}

void delete_node(node *n) {
    node *prev_n = n->prev;
    node *next_n = n->next;
    prev_n->next = next_n;
    next_n->prev = prev_n;
    free_node(n);
}

linked_list *init_linked_list_ret(const size_t element_size) {
    linked_list *ll = malloc(sizeof(linked_list));
    ll->element_size = element_size;

    ll->head = malloc(sizeof(node));
    ll->tail = malloc(sizeof(node));

    ll->head->data = malloc(element_size);
    ll->head->element_size = element_size;
    ll->head->next = ll->tail;
    ll->head->prev = NULL;

    ll->tail->data = malloc(element_size);
    ll->tail->element_size = element_size;
    ll->tail->next = NULL;
    ll->tail->prev = ll->head;

    ll->size = 2;
    return ll;
}

void init_linked_list(linked_list *ll, const size_t element_size) {
    ll->element_size = element_size;

    ll->head = malloc(sizeof(node));
    ll->head->data = malloc(element_size);
    ll->head->element_size = element_size;
    ll->head->next = ll->tail;
    ll->head->prev = NULL;

    ll->tail = malloc(sizeof(node));
    ll->tail->data = malloc(element_size);
    ll->tail->element_size = element_size;
    ll->tail->next = NULL;
    ll->tail->prev = ll->head;

    ll->size = 2;
}

void set_linked_list_head(linked_list *ll, const void *item, const size_t item_size) {
    if (item_size != ll->element_size) {
        fprintf(stderr, "Type mismatch between the item and the vector.\n");
        return;
    }

    if (!ll->head) {
        fprintf(stderr, "Head is not allocated, please initialize the linked list.\n");
        return;
    }
    memcpy(ll->head->data, item, ll->element_size);
}
void set_linked_list_tail(linked_list *ll, const void *item, const size_t item_size) {
    if (item_size != ll->element_size) {
        fprintf(stderr, "Type mismatch between the item and the vector.\n");
        return;
    }

    if (!ll->tail) {
        fprintf(stderr, "Tail is not allocated, please initialize the linked list.\n");
        return;
    }
    memcpy(ll->tail->data, item, ll->element_size);
}

void free_node(node *n) {
    free(n->data);
    free(n);
}

void free_linked_list(linked_list *ll) {
    while (ll->head != NULL) {
        free(ll->head->data);
        free(ll->head);
        ll->head = ll->head->next;
    }
    free(ll);
}

void linked_list_info_log(linked_list *ll) {
    node *tmp = ll->head;
    while (tmp != NULL) {
        printf("%d\n", *(int*)(tmp->data));
        tmp = tmp->next;
    }
}

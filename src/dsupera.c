#include "dsupera.h"

typedef struct vector {
  void *data;
  size_t size;         // stores the num of elements
  size_t max_limit;    // stores max capacity of vec
  size_t element_size; // size of element in bytes
} vector;

void init_vector(vector *vec, const size_t element_size) {
  vec->size = 0;
  vec->max_limit = 4;
  vec->element_size = element_size;
  vec->data = malloc(element_size * vec->max_limit);
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

void vec_push_back(vector *vec, const void *item) {
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

int vec_idx_of_first(vector *vec, const void *item) {
    for (size_t i=0; i<vec->size; ++i) {
        if (memcmp((char*)vec->data+i*vec->element_size, item, vec->element_size)==0) return i;
    }
    return -1;
}

int vec_idx_of_last(vector *vec, const void *item) {
    for (int i=(int)vec->size; i>=0; --i) {
        if (memcmp((char*)vec->data+i*vec->element_size, item, vec->element_size)==0) return i;
    }
    return -1;
}

array vec_all_occurrences_of(vector *vec, const void *item) {
    array occurrence_arr = {
        .data = (int*)malloc(sizeof(int)*vec->size),
        .size = vec->size
    };

    size_t idx=0;
    for (size_t i=0; i<vec->size; ++i) {
        if (memcmp((char*)vec->data+i*vec->element_size, item, vec->element_size)==0) {
            ((int*)occurrence_arr.data)[idx++] = i;
        }
    }
    if (idx < vec->size) {
        occurrence_arr.data = realloc(occurrence_arr.data, sizeof(int)*idx);
        occurrence_arr.size = idx;
    }
    return occurrence_arr;
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

void info_log(const vector *v) {
    printf("vector size -> %zu,\nvector max limit -> %zu,\nvector element size -> %lu,\n", v->size, v->max_limit, v->element_size);
    if (v->size == 0) return;
    for (size_t i = 0; i < v->size; ++i) {
        printf("%zu -> %d\n", i, ((int*)v->data)[i]);
    }
}

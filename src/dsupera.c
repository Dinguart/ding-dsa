#include "dsupera.h"

void init_vector(vector *vec, const size_t element_size) {
  vec->size = 0;
  vec->max_limit = 4;
  vec->element_size = element_size;
  vec->data = malloc(element_size * vec->max_limit);
}

vector init_vector_ret(const size_t element_size) {
    size_t max_limit = 4;
    return (vector) {
        malloc(element_size*max_limit),
        0,
        max_limit,
        element_size,
    };
}

void vec_push_back(vector *vec, const void *item) {
  if (vec->size > vec->max_limit) {
    vec->max_limit *= 2;
    vec->data = realloc(vec->data, vec->max_limit * vec->element_size);
  }

  void *dest = (char *)vec->data + (vec->size * vec->element_size);
  memcpy(dest, item, vec->element_size);
  vec->size++;
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
void vec_reserve(vector *vec, const size_t new_max) {
    vec->data = realloc(vec->data, new_max);
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

array all_occurrences_of(vector *vec, const void *item) {
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

void vec_free(vector *vec) {
    free(vec->data);
    vec->size=0;
    vec->max_limit=0;
    vec->element_size=0;
}

void info_log(const vector *v) {
    printf("vector size -> %zu,\nvector max limit -> %zu,\nvector element size -> %lu,\n", v->size, v->max_limit, v->element_size);
    if (v->size == 0) return;
    for (size_t i = 0; i < v->size; ++i) {
        printf("%zu -> %d\n", i, ((int*)v->data)[i]);
    }
}

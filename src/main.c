#include <stdio.h>
#include "dsupera.h"

void test_front_back() {
    vector *v = init_vector_ret(sizeof(int));
    info_log(v);
    int x = 7;
    vec_push_back(v, &x);
    info_log(v);
    int y = 8;
    vec_push_back(v, &y);
    info_log(v);
    int *z = (int *)vec_front(v);
    printf("\n%d\n", *z);
    info_log(v);
    int *m = (int *)vec_back(v);
    printf("\n%d\n", *m);
}

void test_find() {
    vector *v = init_vector_ret(sizeof(int));

    int a=1, b=2, c=3, d=4;
    vec_push_back(v, &a);
    vec_push_back(v, &b);
    vec_push_back(v, &c);
    vec_push_back(v, &d);
    vec_push_back(v, &c);

    printf("%d\n", vec_idx_of_first(v, &c));
    printf("%d\n", vec_idx_of_last(v, &c));

    array ar = vec_all_occurrences_of(v, &c);
    for (size_t i=0; i<ar.size; ++i) {
        printf("%d\n", ((int*)ar.data)[i]);
    }
    puts("");

    printf("%d\n", *(int*)vec_item_at(v, 2));
}

void test_concat() {
    vector *v1 = init_vector_ret(sizeof(int));

    int a=1, b=2, c=3, d=4;
    vec_push_back(v1, &a);
    vec_push_back(v1, &b);
    vec_push_back(v1, &c);
    vec_push_back(v1, &d);
    vec_push_back(v1, &c);

    vector *v2 = init_vector_ret(sizeof(int));

    vec_push_back(v2, &a);
    vec_push_back(v2, &b);
    vec_push_back(v2, &c);
    vec_push_back(v2, &d);
    vec_push_back(v2, &c);

    vec_concat(v1, v2);
    vec_shrink_to_fit(v1);
    info_log(v1);
    info_log(v2);
}

void test_mem_funcs() {
    vector *v = init_vector_ret(sizeof(int));

    int a=1, b=2, c=3, d=4;
    vec_push_back(v, &a);
    vec_push_back(v, &b);
    vec_push_back(v, &c);
    vec_push_back(v, &d);
    vec_push_back(v, &c);

    vec_pop_back(v);
    info_log(v);
}

int main(void) {
    //    test_front_back();
    //test_find();
    //test_concat();
    test_mem_funcs();
}

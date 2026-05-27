#include <stdio.h>
#include "dsupera.h"

void test_front_back() {
    vector v = init_vector_ret(sizeof(int));
    info_log(&v);
    int x = 7;
    vec_push_back(&v, &x);
    info_log(&v);
    int y = 8;
    vec_push_back(&v, &y);
    info_log(&v);
    int *z = (int *)vec_front(&v);
    printf("\n%d\n", *z);
    info_log(&v);
    int *m = (int *)vec_back(&v);
    printf("\n%d\n", *m);
}

void test_find() {
    vector v = init_vector_ret(sizeof(int));

    int a=1, b=2, c=3, d=4;
    vec_push_back(&v, &a);
    vec_push_back(&v, &b);
    vec_push_back(&v, &c);
    vec_push_back(&v, &d);
    vec_push_back(&v, &c);

    printf("%d\n", vec_idx_of_first(&v, &c));
    printf("%d\n", vec_idx_of_last(&v, &c));

    array ar = all_occurrences_of(&v, &c);
    for (size_t i=0; i<ar.size; ++i) {
        printf("%d\n", ((int*)ar.data)[i]);
    }
}

int main(void) {
    //    test_front_back();
    test_find();
}

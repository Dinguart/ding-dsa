#include <stdio.h>
#include "dsupera.h"

void test_front_back() {
    vector *v = init_vector_ret(sizeof(int));
    vec_info_log(v);
    int x = 7;
    vec_push_back(v, &x, sizeof(int));
    vec_info_log(v);
    int y = 8;
    vec_push_back(v, &y, sizeof(int));
    vec_info_log(v);
    int *z = (int *)vec_front(v);
    printf("\n%d\n", *z);
    vec_info_log(v);
    int *m = (int *)vec_back(v);
    printf("\n%d\n", *m);

    vec_free(v);
}

void test_find() {
    vector *v = init_vector_ret(sizeof(int));

    int a=1, b=2, c=3, d=4;
    vec_push_back(v, &a, sizeof(int));
    vec_push_back(v, &b, sizeof(int));
    vec_push_back(v, &c, sizeof(int));
    vec_push_back(v, &d, sizeof(int));
    vec_push_back(v, &c, sizeof(int));

    printf("%d\n", vec_idx_of_first(v, &c, sizeof(int)));
    printf("%d\n", vec_idx_of_last(v, &c, sizeof(int)));

    array ar = {0};
    vec_all_occurrences_of(&ar, v, &c, sizeof(int));
    for (size_t i=0; i<ar.size; ++i) {
        printf("%d\n", ((int*)ar.data)[i]);
    }
    puts("");

    printf("%d\n", *(int*)vec_item_at(v, 2));

    vec_free(v);
}

void test_concat() {
    vector *v1 = init_vector_ret(sizeof(int));

    int a=1, b=2, c=3, d=4;
    vec_push_back(v1, &a, sizeof(int));
    vec_push_back(v1, &b, sizeof(int));
    vec_push_back(v1, &c, sizeof(int));
    vec_push_back(v1, &d, sizeof(int));
    vec_push_back(v1, &c, sizeof(int));

    vector *v2 = init_vector_ret(sizeof(int));

    vec_push_back(v2, &a, sizeof(int));
    vec_push_back(v2, &b, sizeof(int));
    vec_push_back(v2, &c, sizeof(int));
    vec_push_back(v2, &d, sizeof(int));
    vec_push_back(v2, &c, sizeof(int));

    vec_concat(v1, v2);
    vec_shrink_to_fit(v1);
    vec_info_log(v1);
    vec_info_log(v2);

    vec_free(v1);
    vec_free(v2);
}

void test_mem_funcs() {
    vector *v = init_vector_ret(sizeof(int));

    int a=1, b=2, c=3, d=4;
    vec_push_back(v, &a, sizeof(int));
    vec_push_back(v, &b, sizeof(int));
    vec_push_back(v, &c, sizeof(int));
    vec_push_back(v, &d, sizeof(int));
    vec_push_back(v, &c, sizeof(int));

    vec_pop_back(v);
    vec_info_log(v);

    vec_free(v);
}

void test_linked_list_init() {
    linked_list *l = init_linked_list_ret(sizeof(int));
    int a=1;
    int b=2;
    int c=3;
    int d=4;

    node *a_node = init_node_ret(&a, sizeof(int));
    node *b_node = init_node_ret(&b, sizeof(int));
    node *c_node = init_node_ret(&c, sizeof(int));
    node_push_front(l, a_node);
    node_push_front(l, b_node);
    node_push_front(l, c_node);

    node *d_node = NULL;
    init_node(&d_node, &d, sizeof(int));
    node_push_back(l, d_node);

    linked_list_info_log(l);

    delete_node(l, a_node);
    linked_list_info_log(l);

    free_linked_list(l);
}

void test_linked_list_free() {
    linked_list *l = init_linked_list_ret(sizeof(int));
    int a=1;
    int b=2;
    int c=3;
    node *a_node = init_node_ret(&a, sizeof(int));
    node *b_node = init_node_ret(&b, sizeof(int));
    node *c_node = init_node_ret(&c, sizeof(int));
    node_push_front(l, a_node);
    node_push_front(l, b_node);
    node_push_front(l, c_node);

    linked_list_info_log(l);
    free_linked_list(l);
}

void test_queue_init() {
    queue *q = init_queue_ret(sizeof(int));
    int a=1;
    int b=2;
    int c=3;
    queue_push(q, &a, sizeof(int));
    queue_push(q, &b, sizeof(int));
    queue_push(q, &c, sizeof(int));

    queue_pop(q);


    queue_info_log(q);
    queue_free(q);
}

int main(void) {
    //    test_front_back();
    //test_find();
    //test_concat();
    //test_mem_funcs();
    //test_linked_list_init(); // fix ordering of ll add node.
    //test_linked_list_free();
    test_queue_init();
}

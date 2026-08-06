#include <stdio.h>
#include "dsupera.h"

void test_front_back() {
    vector *v = init_vector_ret(&TI_INT);
    vec_info_log(v);
    int x = 7;
    vec_push_back(v, &x, &TI_INT);
    vec_info_log(v);
    int y = 8;
    vec_push_back(v, &y, &TI_INT);
    vec_info_log(v);
    int *z = (int *)vec_front(v);
    printf("\n%d\n", *z);
    vec_info_log(v);
    int *m = (int *)vec_back(v);
    printf("\n%d\n", *m);

    vec_free(v);
}

void test_find() {
    vector *v = init_vector_ret(&TI_INT);

    int a=1, b=2, c=3, d=4;
    vec_push_back(v, &a, &TI_INT);
    vec_push_back(v, &b, &TI_INT);
    vec_push_back(v, &c, &TI_INT);
    vec_push_back(v, &d, &TI_INT);
    vec_push_back(v, &c, &TI_INT);

    printf("%d\n", vec_idx_of_first(v, &c, &TI_INT));
    printf("%d\n", vec_idx_of_last(v, &c, &TI_INT));


    vector *ar = vec_all_occurrences_of(v, &c, &TI_INT);
    vec_info_log(ar);
    puts("");

    printf("%d\n", *(int*)vec_item_at(v, 2));

    vec_free(v);
}

void test_concat() {
    vector *v1 = init_vector_ret(&TI_INT);

    int a=1, b=2, c=3, d=4;
    vec_push_back(v1, &a, &TI_INT);
    vec_push_back(v1, &b, &TI_INT);
    vec_push_back(v1, &c, &TI_INT);
    vec_push_back(v1, &d, &TI_INT);
    vec_push_back(v1, &c, &TI_INT);

    vector *v2 = init_vector_ret(&TI_INT);

    vec_push_back(v2, &a, &TI_INT);
    vec_push_back(v2, &b, &TI_INT);
    vec_push_back(v2, &c, &TI_INT);
    vec_push_back(v2, &d, &TI_INT);
    vec_push_back(v2, &c, &TI_INT);

    vec_concat(v1, v2);
    vec_shrink_to_fit(v1);
    vec_info_log(v1);
    vec_info_log(v2);

    vec_free(v1);
    vec_free(v2);
}

void test_mem_funcs() {
    vector *v = init_vector_ret(&TI_INT);

    int a=1, b=2, c=3, d=4;
    vec_push_back(v, &a, &TI_INT);
    vec_push_back(v, &b, &TI_INT);
    vec_push_back(v, &c, &TI_INT);
    vec_push_back(v, &d, &TI_INT);
    vec_push_back(v, &c, &TI_INT);

    vec_pop_back(v);
    vec_info_log(v);

    vec_free(v);
}

void test_linked_list_init() {
    linked_list *l = init_linked_list_ret(&TI_INT);
    int a=1;
    int b=2;
    int c=3;
    int d=4;

    node *a_node = init_node_ret(&a, &TI_INT);
    node *b_node = init_node_ret(&b, &TI_INT);
    node *c_node = init_node_ret(&c, &TI_INT);
    node_push_front(l, a_node);
    node_push_front(l, b_node);
    node_push_front(l, c_node);

    node *d_node = NULL;
    init_node(&d_node, &d, &TI_INT);
    node_push_back(l, d_node);

    linked_list_info_log(l);

    delete_node(l, a_node);
    linked_list_info_log(l);

    free_linked_list(l);
}

void test_linked_list_free() {
    linked_list *l = init_linked_list_ret(&TI_INT);
    int a=1;
    int b=2;
    int c=3;
    node *a_node = init_node_ret(&a, &TI_INT);
    node *b_node = init_node_ret(&b, &TI_INT);
    node *c_node = init_node_ret(&c, &TI_INT);
    node_push_front(l, a_node);
    node_push_front(l, b_node);
    node_push_front(l, c_node);

    linked_list_info_log(l);
    free_linked_list(l);
}

void test_queue_init() {
    queue *q = init_queue_ret(&TI_INT);
    int a=1;
    int b=2;
    int c=3;
    queue_push(q, &a, &TI_INT);
    queue_push(q, &b, &TI_INT);
    queue_push(q, &c, &TI_INT);

    queue_pop(q);


    queue_info_log(q);
    queue_free(q);
}

void test_pair_init() {
    int a=1;
    char *b="Fuck you";
    pair *p = init_pair_ret(&a, &TI_INT, &b, &TI_STRING);
    pair_info_log(p);
    free(p);
}

void test_umap_init() {
    unorder_map *u = init_u_map_ret(&TI_INT, &TI_INT);
    int a_key = 1;
    int a_val = 10;
    int b_key = 2;
    int b_val = 20;
    int c_key = 3;
    int c_val = 30;

    if (!umap_insert(u, &a_key, &TI_INT, &a_val, &TI_INT)) return;
    int *supposed_a_val = (int*)umap_getval(u, &a_key, &TI_INT);
    umap_free(u);
    if (!supposed_a_val) return;
    printf("%d equals %d\n", a_val, *supposed_a_val);
}

int main(void) {
    //test_front_back();
    //test_find();
    //test_concat();
    //test_mem_funcs();
    //test_linked_list_init(); // fix ordering of ll add node.
    //test_linked_list_free();
    //test_queue_init();
    //test_pair_init();
    test_umap_init();
}

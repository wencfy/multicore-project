#include <omp.h>

typedef struct binary_tree_node_s {
    int value;
    struct binary_tree_node_s *left, *right;
    omp_lock_t lock;
} binary_tree_node_t;

typedef struct binary_tree_s {
    binary_tree_node_t *root;
    omp_lock_t lock;
} binary_tree_t;

binary_tree_t *new_binary_tree();

binary_tree_node_t *new_tree_node(int value);

void binary_tree_insert_serial(binary_tree_t* tree, binary_tree_node_t* to_insert);

void binary_tree_insert(binary_tree_t* tree, binary_tree_node_t* to_insert);

binary_tree_node_t* binary_tree_search(binary_tree_t* tree, int value);

void in_order_traverse(binary_tree_node_t* tree);

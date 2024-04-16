#include <stdio.h>

#include "tree.h"

binary_tree_t *new_binary_tree() {
    binary_tree_t *tree = (binary_tree_t *) malloc(sizeof(binary_tree_t));
    tree->root = NULL;
    omp_init_lock(&tree->lock);
    return tree;
}

binary_tree_node_t *new_tree_node(int value) {
    binary_tree_node_t *node = (binary_tree_node_t *) malloc(sizeof(binary_tree_node_t));
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void binary_tree_insert_serial(binary_tree_t* tree, binary_tree_node_t* to_insert) {
    if (!(tree->root)) {
        tree->root = to_insert;
        return ;
    }

    binary_tree_node_t *parent = NULL;
    binary_tree_node_t *current = tree->root;
    while (current != NULL) {
        parent = current;
        if (to_insert->value < current->value) {
            current = current ->left;
            if (current == NULL) {
                parent->left = to_insert;
                return ;
            }
        } else {
            current = current->right;
            if (current == NULL) {
                parent->right = to_insert;
                return ;
            }
        }
    }
}

binary_tree_node_t* binary_tree_search(binary_tree_t* tree, int value) {
    binary_tree_node_t* current = tree->root;
    while (current != NULL) {
        if (value < current->value) {
            current = current ->left;
        } else if (value > current->value) {
            current = current->right;
        } else {
            // found
            return current;
        }
    }
    return NULL;
}

void in_order_traverse(binary_tree_node_t* tree) {
    if (tree) {
        in_order_traverse(tree->left);
        printf("%d\n", tree->value);
        in_order_traverse(tree->right);
    }
}

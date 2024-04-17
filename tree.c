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
    omp_init_lock(&node->lock);
    return node;
}

void binary_tree_insert_serial(binary_tree_t* tree, binary_tree_node_t* to_insert) {
    if (!(tree->root)) {
        if (!(tree->root)) {
            tree->root = to_insert;
            return ;
        }
    }

    binary_tree_node_t *current = tree->root;
    while (current != NULL) {
        if (to_insert->value < current->value) {
            if (current->left == NULL) {
                current->left = to_insert;
                break;
            }
            current = current->left;
        } else {
            if (current->right == NULL) {
                current->right = to_insert;
                break;
            }
            current = current->right;
        }
    }
}

void binary_tree_insert(binary_tree_t* tree, binary_tree_node_t* to_insert) {
    if (!(tree->root)) {
        omp_set_lock(&tree->lock);
        if (!(tree->root)) {
            tree->root = to_insert;
            omp_unset_lock(&tree->lock);
            return ;
        }
        omp_unset_lock(&tree->lock);
    }

    binary_tree_node_t *current = tree->root;
    omp_set_lock(&current->lock);

    while (current != NULL) {
        if (to_insert->value < current->value) {
            if (current->left == NULL) {
                current->left = to_insert;
                omp_unset_lock(&current->lock);
                break;
            } else {
                binary_tree_node_t *next = current->left;
                omp_set_lock(&next->lock);
                omp_unset_lock(&current->lock);
                current = next;
            }
        } else {
            if (current->right == NULL) {
                current->right = to_insert;
                omp_unset_lock(&current->lock);
                break;
            } else {
                binary_tree_node_t *next = current->right;
                omp_set_lock(&next->lock);
                omp_unset_lock(&current->lock);
                current = next;
            }
        }
    }
}

bool binary_tree_delete(binary_tree_t *tree, int data) {
    binary_tree_node_t *current = tree->root;
    binary_tree_node_t *parent = NULL;

    omp_set_lock(&current->lock);
    // Find the node and its parent
    while (current != NULL && current->value != data) {
        if (parent) {
            omp_unset_lock(&parent->lock); // Ensure to unlock previous parent
        }
        parent = current;

        if (data < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }

        if (current) {
            omp_set_lock(&current->lock); // Lock the next node
        }
    }
    // here, parent and current are both locked.

    if (current == NULL) {  // Data not found
        if (parent) {
            omp_unset_lock(&parent->lock);
        }
        return false;
    }

    // Handle node with two children
    if (current->left && current->right) {
        binary_tree_node_t *succ = current->right;
        binary_tree_node_t *succ_parent = current;

        omp_set_lock(&succ->lock);

        // Find the in-order successor
        while (succ->left) {
            succ_parent = succ;
            succ = succ->left;
            omp_set_lock(&succ->lock); // Lock successor
            omp_unset_lock(&succ_parent->lock);
        }

        // Swap values
        int temp = current->value;
        current->value = succ->value;
        succ->value = temp;

        omp_unset_lock(&current->lock);
        current = succ; // current points to successor which will be deleted

        if (parent) {
            omp_unset_lock(&parent->lock);
        }
        parent = succ_parent;
        omp_set_lock(&parent->lock);
    }

    // Deleting the node (current) which is now guaranteed to have at most one child
    binary_tree_node_t *child = (current->left) ? current->left : current->right;

    if (parent) {
        if (parent->left == current) {
            parent->left = child;
        } else {
            parent->right = child;
        }
        omp_unset_lock(&parent->lock); // Unlock parent
    } else {
        tree->root = child; // current is root
    }

    omp_unset_lock(&current->lock);
    omp_destroy_lock(&current->lock); // Destroy lock of the node being deleted
    free(current); // Free node space
    return true;
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

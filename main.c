#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "tree.h"

#define MAX_LINES 2000000

int main(int argc, char *argv[]) {
    // Check if a filename is provided
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[2], "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char line[256];
    int numbers[MAX_LINES];
    int count = 0;

    // Read each line from the file and convert to an integer
    while (fgets(line, sizeof(line), file) && count < MAX_LINES) {
        numbers[count] = atoi(line);
        count++;
    }

    // Close the file
    fclose(file);

    int num_threads = atoi(argv[1]);
    omp_set_num_threads(num_threads);

    binary_tree_t* tree = new_binary_tree();

    // Insert all numbers to the tree
    // for (int i = 0; i < count; i++) {
    //     binary_tree_insert_serial(tree, new_tree_node(numbers[i]));
    // }

    #pragma omp parallel for
    for (int i = 0; i < count; i++) {
        binary_tree_insert(tree, new_tree_node(numbers[i]));
    }

    // in_order_traverse(tree->root);

    // #pragma omp parallel for
    // for (int i = 0; i < count; i++) {
    //     binary_tree_node_t* node = binary_tree_search(tree, numbers[i]);
    //     // printf("%d found: %d\n", numbers[i], node->value);
    // }

    // printf("\n----------------------\n");
    #pragma omp parallel for
    for (int i = 0; i < count; i++) {
        binary_tree_delete(tree, numbers[i]);
    }

    // in_order_traverse(tree->root);

    return EXIT_SUCCESS;
}

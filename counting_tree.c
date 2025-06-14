#include <stdio.h>
#include <stdlib.h>
#include "huffman_coding.h"

struct node *add_tree(struct node *root, char symbol) {
    if (!root) {
	root = (struct node *) malloc(sizeof(struct node));
	root->symbol = symbol;
	root->count = 1;
	root->left = root->right = NULL;
	return root;
    }
    if (root->symbol == symbol) {
        root->count++;
    } else if (symbol < root->symbol) {
        root->left = add_tree(root->left, symbol);
    } else {
        root->right = add_tree(root->right, symbol);
    }
    return root;
}

void tree_print(struct node *root) {
    if (root != NULL) {
        tree_print(root->left);
        printf("%c %d\n", root->symbol, root->count);
        tree_print(root->right);
    }
}

int collect_nodes(struct leaf arr[], struct node *root, int index) {
    if (root != NULL) {
	index = collect_nodes(arr, root->left, index);
	struct leaf new_leaf = {root->count, root->symbol};
	arr[index++] = new_leaf;
	index = collect_nodes(arr, root->right, index);
    }
    return index;
}

void free_tree(struct node *root) {
    if (root != NULL) {
	free_tree(root->left);
	free_tree(root->right);
	free(root);
    }
}

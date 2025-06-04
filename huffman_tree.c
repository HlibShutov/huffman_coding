#include <stdlib.h>
#include <stdio.h>
#include "huffman_coding.h"

int compare(const void *a, const void *b) {
    const struct leaf *na = (const struct leaf *)a;
    const struct leaf *nb = (const struct leaf *)b;
    return na->weight - nb->weight;
}

int leafs_index = 0;

struct internal *build_tree(struct leaf leafs[], int leafs_count) {
    struct queue working_queue = {NULL, NULL};
    while (leafs_index < leafs_count || len(&working_queue) > 1) {
	struct huffman_node *left, *right;
	struct huffman_node *min1 = get_min(leafs, leafs_count, &working_queue);
	struct huffman_node *min2 = get_min(leafs, leafs_count, &working_queue);

        struct internal *merged = malloc(sizeof(struct internal));
	merged->left = min1;
	merged->right = min2;
        merged->weight = get_weight(min1) + get_weight(min2);
        enqueue(&working_queue, merged);
    }
        
    struct internal *final = dequeue(&working_queue);
    return final;
}

int get_weight(struct huffman_node *node) {
    return (node->type == LEAF) ? node->value.leaf_node.weight : node->value.internal_node->weight;
}

struct huffman_node *get_min(struct leaf leafs[], int leafs_count, struct queue *working_queue) {
    struct huffman_node *ret = (struct huffman_node *) malloc(sizeof(struct huffman_node));

    if (working_queue->first == NULL) {
        ret->value.leaf_node = leafs[leafs_index++];
	ret->type = LEAF;
	return ret;
    }
    if (leafs_index >= leafs_count) {
	ret->value.internal_node = dequeue(working_queue);
	ret->type = INTERNAL;
	return ret;

    }

    if (leafs[leafs_index].weight <= working_queue->first->value->weight) {
        ret->value.leaf_node = leafs[leafs_index++];
	ret->type = LEAF;
	return ret;
    }
    else {
        ret->value.internal_node = dequeue(working_queue);
	ret->type = INTERNAL;
	return ret;
    }
}

void print_huffman_tree(struct internal *root, int offset) {
    if (root->left->type == LEAF) {
	for (int i = 0; i < offset*4; i++)
	    printf(" ");
	printf("left leaf '%c' %d\n", root->left->value.leaf_node.symbol, root->left->value.leaf_node.weight);
    } else
	print_huffman_tree(root->left->value.internal_node, ++offset);

    if (root->right->type == LEAF) {
	for (int i = 0; i < offset*4; i++)
	    printf(" ");
	printf("right leaf '%c' %d\n", root->right->value.leaf_node.symbol, root->right->value.leaf_node.weight);
    } else
	print_huffman_tree(root->right->value.internal_node, ++offset);
}

void free_huffman_tree(struct internal *root) {
    if (root != NULL) {
        if (root->left->type == INTERNAL)
            free_huffman_tree(root->left->value.internal_node);
        free(root->left);

        if (root->right->type == INTERNAL)
            free_huffman_tree(root->right->value.internal_node);

        free(root->right);
        free(root);
    }
}

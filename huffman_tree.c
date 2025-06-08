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

void collect_huffman_tree(struct internal *root, struct bit_path dict[], struct bit_path path) {
    struct bit_path left_path = path;
    add_bit(&left_path, 0);
    if (root->left->type == LEAF) {
        dict[root->left->value.leaf_node.symbol] = left_path;
    } else {
        collect_huffman_tree(root->left->value.internal_node, dict, left_path);
    }

    struct bit_path right_path = path;
    add_bit(&right_path, 1);
    if (root->right->type == LEAF) {
        dict[root->right->value.leaf_node.symbol] = right_path;
    } else {
        collect_huffman_tree(root->right->value.internal_node, dict, right_path);
    }
}


void add_bit(struct bit_path *path, int bit) {
    path->bits |= ((char)(bit & 1) << (7 - path->length));
    path->length++;
}

void write_huffman_tree(struct bit_writer *writer, struct huffman_node node) {
    if (node.type == LEAF) {
	write_bits(writer, (char)(1 << 7), 1);
	write_bits(writer, node.value.leaf_node.symbol, 8);
    } else {
	write_bits(writer, 0, 1);
	write_huffman_tree(writer, *node.value.internal_node->left);
	write_huffman_tree(writer, *node.value.internal_node->right);
    }
}

struct huffman_node *parse_huffman(struct bit_reader *reader) {
    unsigned char size[4];
    int sum = 0;
    for (int i = 0; i < 4; i++) {
	size[i] = read_byte(reader);
	if (size[i] == 0)
	    break;
        sum += size[0] | (size[1]<<8) | (size[2]<<16) | (size[3]<<24);
    }
    printf("size little endian %d\n", sum);
    reader->byte_pos = 4 + sum / 8;

    struct huffman_node *root = read_node(reader);
    struct huffman_node *current_node = root;
    print_huffman_tree(root->value.internal_node, 0);
    reader->byte_pos = 4;
    while (read_bit(reader) == 1)
	;

    printf("result\n");
    while ((reader->byte_pos * 8 + reader->bit_pos) < (4 + sum / 8)*8) {
	if (read_bit(reader) == 0)
	    current_node = current_node->value.internal_node->left;
	else
	    current_node = current_node->value.internal_node->right;
	if (current_node->type == LEAF) {
	    putchar(current_node->value.leaf_node.symbol);
	    current_node = root;
	}
    } 
    return root;
}

struct huffman_node *read_node(struct bit_reader *reader) {
    struct huffman_node *ret = (struct huffman_node *) malloc(sizeof(struct huffman_node));
    if (read_bit(reader) == 1) {
	struct leaf leaf_node;
	leaf_node.symbol = read_byte(reader);
	ret->value.leaf_node = leaf_node;
	ret->type = LEAF;
	return ret;
    } else {
	struct huffman_node *left = read_node(reader);
	struct huffman_node *right = read_node(reader);
	struct internal *internal_node = (struct internal *) malloc(sizeof(struct internal));
	internal_node->left = left;
	internal_node->right = right;
	ret->value.internal_node = internal_node;
	ret->type = INTERNAL;
	return ret;
    }
}

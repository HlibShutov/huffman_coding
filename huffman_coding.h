#pragma once
struct node {
    int count;
    struct node *left;
    struct node *right;
    char symbol;
};

struct leaf {
    int weight;
    char symbol;
};

struct internal {
    union huffman_node *left;
    union huffman_node *right;
    int weight;
};

union huffman_node {
    struct leaf leaf_node;
    struct internal internal_node;
};

struct node *add_tree(struct node *, char);
void tree_print(struct node *);
int collect_nodes(struct leaf arr[], struct node *root, int index);
void free_tree(struct node *);
int compare(const void *, const void *);

struct queue_node {
    struct internal value;
    struct queue_node *next;
};
struct queue {
    struct queue_node *first;
    struct queue_node *last;
};

void enqueue(struct queue *working_queue, struct internal value);
struct internal dequeue(struct queue *working_queue);

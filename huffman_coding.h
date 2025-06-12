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
    struct huffman_node *left;
    struct huffman_node *right;
    int weight;
};

enum NodeType {
    LEAF,
    INTERNAL,
};
struct huffman_node {
    union {
        struct leaf leaf_node;
        struct internal *internal_node;
    } value;
    enum NodeType type;
};

struct node *add_tree(struct node *, char);
void tree_print(struct node *);
int collect_nodes(struct leaf[], struct node *, int);
void free_tree(struct node *);
int compare(const void *, const void *);

struct queue_node {
    struct internal *value;
    struct queue_node *next;
};
struct queue {
    struct queue_node *first;
    struct queue_node *last;
};

void enqueue(struct queue *, struct internal *);
struct internal *dequeue(struct queue *);
int len(struct queue *);

struct internal *build_tree(struct leaf[], int leafs_count);
int get_weight(struct huffman_node *);
struct huffman_node *get_min(struct leaf[], int, struct queue *);
void print_huffman_tree(struct internal *, int);
void free_huffman_tree(struct internal *root);

struct bit_path {
    int bits;
    int length;
};

void add_bit(struct bit_path *, int);
int collect_huffman_tree(struct internal *, struct bit_path[], struct bit_path);

struct bit_writer {
    FILE *fp;
    int bits_present;
    unsigned char current_byte;
};

struct bit_writer open_bit_writer(char *, int, char);
void write_bits(struct bit_writer *, char, int);
void close_bit_writer(struct bit_writer *);
void print_binary(char, int);
void write_huffman_tree(struct bit_writer *, struct huffman_node);

#define BUFFER_SIZE 32

struct bit_reader {
    FILE *fp;
    unsigned char buffer[BUFFER_SIZE];
    int buffer_len;
    int byte_pos;
    int bit_pos;
    int eof;
};

char read_bit(struct bit_reader *);
unsigned char read_byte(struct bit_reader *);

struct huffman_node *parse_huffman(struct bit_reader *, FILE *);
struct huffman_node *read_node(struct bit_reader *);

void print_binary_int(int, int);
void write_bits_int(struct bit_writer *, int, int);
void refill_buffer(struct bit_reader *);

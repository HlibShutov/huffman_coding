struct node {
    int count;
    struct node *left;
    struct node *right;
    char symbol;
};

struct node *add_tree(struct node *, char);
void tree_print(struct node *);
int collect_nodes(struct node *arr[], struct node *root, int index);
void free_tree(struct node *);
int compare(const void *, const void *);

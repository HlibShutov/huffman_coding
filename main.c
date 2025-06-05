#include <stdio.h>
#include <stdlib.h>
#include "huffman_coding.h"

int main(int argc, char *argv[]) {
    FILE *fp;
    int c, symbols_count;
    struct node *root = NULL;
    struct leaf leafs[256];
    struct bit_path dict[256];

    if (argc != 2) {
	fprintf(stderr, "provide file name\n");
	exit(1);
    } else
	fp = fopen(argv[1], "r");


    while ((c = getc(fp)) != EOF)
	root = add_tree(root, c);



    /* tree_print(root); */

    printf("collected: \n\n\n");
    symbols_count = collect_nodes(leafs, root, 0);
    free_tree(root);
    qsort(leafs, symbols_count, sizeof(leafs[0]), compare);
    for (int i = 0; i < symbols_count; i++)
	printf("'%c' %d\n", leafs[i].symbol, leafs[i].weight);

    struct internal *huffman_tree = build_tree(leafs, symbols_count);
    print_huffman_tree(huffman_tree, 0);
    printf("Huffman tree built. Root weight: %d\n", huffman_tree->weight);
    struct bit_path path = {0, 0};
    collect_huffman_tree(huffman_tree, dict, path);

    rewind(fp);
    while ((c = getc(fp)) != EOF) {
	printf("'%c': ", c);
	print_path(dict[c]);
    }

    fclose(fp);
    free_huffman_tree(huffman_tree);

    return 0;
}

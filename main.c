#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
    int size = 0;
    while ((c = getc(fp)) != EOF) {
	printf("'%c': ", c);
	print_binary(dict[c].bits, dict[c].length);
	printf("\n");
	size += dict[c].length;
    }
    printf("bits size %d\n", size);

    FILE *output = fopen("example.huffman", "wb");
    struct bit_writer writer = {output, 0, 0};
    int closest_num = size;

    if (size % 8 != 0) {
        closest_num = ((int)(size / 8) + 1) * 8;
        int bits_present = closest_num - size;
        printf("present %d closest %d size %d\n\n\n", bits_present, closest_num, size);
        char current_byte = 0xFF << (8 - (bits_present - 1));
        writer.bits_present = bits_present;
        writer.current_byte = current_byte;
    }


    fwrite(&closest_num, sizeof(int), 1, output);
    printf("size of int %zu, size of encoded %d\n", sizeof(int), closest_num);

    rewind(fp);
    while ((c = getc(fp)) != EOF)
        write_bits(&writer, dict[c].bits, dict[c].length);

    fclose(fp);

    struct huffman_node node;
    node.value.internal_node = huffman_tree;
    node.type = 1;
    write_huffman_tree(&writer, node);
    close_bit_writer(&writer);
    free_huffman_tree(huffman_tree);

    printf("output\n");
    FILE *fp1 = fopen("example.huffman", "r");
    while ((c = getc(fp1)) != EOF) {
	print_binary(c, 8);
    }
    printf("\n");
    fclose(fp1);
    return 0;
}

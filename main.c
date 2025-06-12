#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "huffman_coding.h"

int main(int argc, char *argv[]) {
    FILE *fp;
    int c, symbols_count, decode;
    struct node *root = NULL;
    struct leaf leafs[256];
    struct bit_path dict[256];

    if (argc == 3) {
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
	int tree_size = collect_huffman_tree(huffman_tree, dict, path);

        rewind(fp);
        int size = 0;
        while ((c = getc(fp)) != EOF) {
    	    printf("'%c': ", c);
    	    print_binary_int(dict[c].bits, dict[c].length);
    	    printf("\n");
    	    size += dict[c].length;
        }
        printf("bits size %d\n", size);

	FILE *output = fopen(argv[2], "wb");
        struct bit_writer writer = {output, 0, 0};

        int file_size = size + tree_size;

        if (file_size % 8 != 0) {
            int closest_num = ((int)(file_size / 8) + 1) * 8;
            int bits_present = closest_num - file_size;
            printf("present %d closest %d size %d\n\n\n", bits_present, closest_num, file_size);
            char current_byte = 0xFF << (8 - (bits_present - 1));
            writer.bits_present = bits_present;
            writer.current_byte = current_byte;
        } else {
            writer.current_byte = 0xFE;
            writer.bits_present = 8;
	}

        rewind(fp);

        struct huffman_node node;
        node.value.internal_node = huffman_tree;
        node.type = 1;
	
        write_huffman_tree(&writer, node);
        while ((c = getc(fp)) != EOF)
            write_bits_int(&writer, dict[c].bits, dict[c].length);

        fclose(fp);
        free_huffman_tree(huffman_tree);

        return 0;
    } else if (argc == 2) {
        fp = fopen(argv[1], "rb");

        fseek(fp, 0, SEEK_END);
        int size = ftell(fp);
	printf("size %d\n", size);
        rewind(fp);
	printf("\n");
	struct bit_reader reader;
	reader.fp = fp;
	reader.buffer_len = 0;
	reader.byte_pos = 0;
	reader.bit_pos = 0;
	reader.eof = 0;
	refill_buffer(&reader);
	struct huffman_node *node = parse_huffman(&reader);
	free_huffman_tree(node->value.internal_node);
	free(node);
    } else {
	fprintf(stderr, "provide file name\n");
	exit(1);
    } 
}

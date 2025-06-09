#include <stdio.h>
#include "huffman_coding.h"

char read_bit(struct bit_reader *reader) {
    char byte = reader->buffer[reader->byte_pos];
    char bit = (byte >> (7 - reader->bit_pos++)) & 1;
    if (reader->bit_pos == 8) {
	reader->byte_pos++;
	reader->bit_pos = 0;
    }
    return bit;
}

unsigned char read_byte(struct bit_reader *reader) {
    unsigned char byte;
    if (reader->bit_pos == 0) {
	byte = reader->buffer[reader->byte_pos++];
    } else {
	unsigned char left_part = reader->buffer[reader->byte_pos++] << reader->bit_pos;
	unsigned char right_part = reader->buffer[reader->byte_pos] >> (8 - reader->bit_pos);
	byte = left_part | right_part;
    }
    return byte;
}

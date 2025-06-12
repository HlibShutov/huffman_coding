#include <stdio.h>
#include "huffman_coding.h"

char read_bit(struct bit_reader *reader) {
    refill_buffer(reader);
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
	refill_buffer(reader);
	byte = reader->buffer[reader->byte_pos++];
    } else {
	refill_buffer(reader);
	unsigned char left_part = reader->buffer[reader->byte_pos++] << reader->bit_pos;
	unsigned char right_part = reader->buffer[reader->byte_pos] >> (8 - reader->bit_pos);
	byte = left_part | right_part;
    }
    return byte;
}

void refill_buffer(struct bit_reader *reader) {
    if (reader->byte_pos >= reader->buffer_len && !reader->eof) {
        reader->buffer_len = fread(reader->buffer, 1, BUFFER_SIZE, reader->fp);
        reader->byte_pos = 0;
        if (reader->buffer_len < BUFFER_SIZE) {
            reader->eof = 1;
        }
    }
}

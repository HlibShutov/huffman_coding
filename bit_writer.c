#include <stdio.h>
#include <math.h>
#include "huffman_coding.h"

void write_bits(struct bit_writer *writer, char bits, int length) {
    while (length > 0) {
        int space_left = 8 - writer->bits_present;

        if (length <= space_left) {
            unsigned char shifted_bits = (unsigned char)bits >> writer->bits_present;
            writer->current_byte |= shifted_bits;
            writer->bits_present += length;
            return;
        } else {
            unsigned char shifted_bits = (unsigned char)bits >> writer->bits_present;
            writer->current_byte |= shifted_bits;
            fwrite(&(writer->current_byte), 1, 1, writer->fp);
            writer->current_byte = 0;
            writer->bits_present = 0;
            bits = (unsigned char)bits << space_left;
            length -= space_left;
        }
    }
}

void write_bits_int(struct bit_writer *writer, int bits, int length) {
    int bits_written = 0;

    while (bits_written < length) {
        int space_left = 8 - writer->bits_present;
        int bits_to_write = length - bits_written;

        if (bits_to_write > space_left)
            bits_to_write = space_left;

        for (int i = 0; i < bits_to_write; i++) {
            int bit_pos = 31 - (bits_written + i);
            int bit = (bits >> bit_pos) & 1;
            int target_bit_pos = 7 - writer->bits_present;

            if (bit)
                writer->current_byte |= (1 << target_bit_pos);
            else
                writer->current_byte &= ~(1 << target_bit_pos);

            writer->bits_present++;
        }

        bits_written += bits_to_write;
        if (writer->bits_present == 8) {
            fwrite(&writer->current_byte, 1, 1, writer->fp);
            writer->current_byte = 0;
            writer->bits_present = 0;
        }
    }
}

void close_bit_writer(struct bit_writer *writer) {
    if (writer->bits_present > 0)
	fwrite(&(writer->current_byte), 1, 1, writer->fp);
    fclose(writer->fp);
}

void print_binary(char c, int length) {
    for (int i = 0; i < length; i++) {
        int shift = 7 - i;
        int bit = (c >> shift) & 1;
        putchar(bit ? '1' : '0');
    }
}

void print_binary_int(int c, int length) {
    for (int i = 0; i < length; i++) {
        int shift = 31 - i;
        int bit = (c >> shift) & 1;
        putchar(bit ? '1' : '0');
    }
}

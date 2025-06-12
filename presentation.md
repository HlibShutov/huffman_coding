# Kodowanie Huffmana

* Kompresja bezstratna
* Najczęściej używany znak ma najmniej bitów
* Są bardziej wydajne algorytmy

---

# Kompresja

1. Czytamy plik, patrzymy ile razy jaki znak występuje
2. Sortujemy znaki ze względu na częstotliwość

3. Tworzymy kolejkę dla internal nodów
4. Sprawdzamy początki listy i koleki, bierzemy 2 najmniejsze elementy z których tworzymy nowego internal noda
5. Powtarzamy ten proces do momentu kiedy zostanie 1 internal node, który jest rootem

6. Iterujemy się po drzewie i zbieramy ścieżki
7. Kodujemy plik

---

# Serializacja drzewa

1. Rekursywnie przechodzimy drzewo
2. Jeżeli to jest internal node, zapisujemy 0 i idziemy dalej
3. Jeżeli to jest leaf node, piszemy 1 i znak

---

# Problem zapisywania do pliku

### Jak dodać pojedynczy bit
```c
void add_bit(struct bit_path *path, int bit) {
    path->bits |= ((bit & 1) << (31 - path->length));
    path->length++;
}
```

### Jak zapisywać bity i bajty do pliku
```c
while ((c = getc(fp)) != EOF)
    write_bits_int(&writer, dict[c].bits, dict[c].length);

```

```c
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
```

---

# Dekompresja

### Deserializacja drzewa

```c
struct huffman_node *read_node(struct bit_reader *reader) {
    struct huffman_node *ret = (struct huffman_node *) malloc(sizeof(struct huffman_node));
    if (read_bit(reader) == 1) {
	    struct leaf leaf_node;
	    leaf_node.symbol = read_byte(reader);
	    printf("%c", leaf_node.symbol);
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
```

### Dalej po prostu czytamy bity po kolei i odczytujemy znak z drzewa

---

# Jak czytać bity

```c
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
    if (reader->byte_pos >= reader->buffer_len) {
        reader->buffer_len = fread(reader->buffer, 1, BUFFER_SIZE, reader->fp);
        reader->byte_pos = 0;
        if (reader->buffer_len < BUFFER_SIZE) {
            reader->eof = 1;
        }
    }
}
```

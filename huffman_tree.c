#include "huffman_coding.h"

int compare(const void *a, const void *b) {
    const struct leaf *na = (const struct leaf *)a;
    const struct leaf *nb = (const struct leaf *)b;
    return na->weight - nb->weight;
}

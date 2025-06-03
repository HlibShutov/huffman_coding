#include <stdlib.h>
#include "huffman_coding.h"

void enqueue(struct queue *working_queue, struct internal value) {
    struct queue_node *node = (struct queue_node *) malloc(sizeof(struct queue_node));
    node->value = value;
    node->next = NULL;
    if (working_queue->last != NULL)
        working_queue->last->next = node;
    else
	working_queue->first = working_queue->last = node;
}

struct internal dequeue(struct queue *working_queue) {
    struct internal node = working_queue->first->value;
    struct queue_node *first = working_queue->first;
    working_queue->first = working_queue->first->next;
    free(first);
    return node;
}

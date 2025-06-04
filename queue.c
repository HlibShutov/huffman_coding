#include <stdlib.h>
#include <stdio.h>
#include "huffman_coding.h"

void enqueue(struct queue *working_queue, struct internal *value) {
    struct queue_node *node = (struct queue_node *) malloc(sizeof(struct queue_node));
    node->value = value;
    node->next = NULL;
    if (working_queue->first != NULL) {
        working_queue->last->next = node;
        working_queue->last = node;  // <-- Missing in your code
    } else
        working_queue->first = working_queue->last = node;
}


struct internal *dequeue(struct queue *working_queue) {
    struct internal *node = working_queue->first->value;
    struct queue_node *first = working_queue->first;
    working_queue->first = working_queue->first->next;
    free(first);
    return node;
}

int len(struct queue *working_queue) {
    int i;
    struct queue_node *node = working_queue->first;
    for (i = 0; node != NULL; i++, node = node->next)
	;
    return i;
}

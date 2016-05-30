#include <stdio.h>
#include <stdlib.h>

typedef struct node_queue {
    int key;
    int *data;
    struct node_queue *next;
} node_queue;

node_queue *add(node_queue **queue, int *data);

int *pop(node_queue **queue);

int free_queue(node_queue **queue);

int is_empty(node_queue **queue);

void print_queue(node_queue **queue);

int search(node_queue **queue, int *data, int size);

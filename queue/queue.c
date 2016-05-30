#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

node_queue *add(node_queue **queue, int *data){
    if(queue == NULL || data == NULL) return NULL;

    node_queue *new = (node_queue *) malloc(sizeof(node_queue));
    if(new == NULL) {
        printf("ERROR!\n");
        return NULL;
    }
    new->data = data;
    if(*queue) {
        new->next = (*queue)->next;
        (*queue)->next = new;
    } else {
        new->next = new;
    }
    *queue = new;

    return *queue;
}

int *pop(node_queue **queue){
    if(queue == NULL) return NULL;

    if(*queue == NULL) return NULL;
    else if(*queue == (*queue)->next) {
        int *i = (*queue)->data;
        free(*queue);
        *queue = NULL;
        return i;
    } else {
        node_queue *aux = (*queue)->next;
        (*queue)->next = aux->next;
        int *i = aux->data;
        free(aux);
        return i;
    }
}

int free_queue(node_queue **queue){
    if(queue == NULL) return -1;
    if(*queue == NULL) return 0;

    if((*queue)->next == *queue) {
        free((*queue)->data);
        free(*queue);
        *queue = NULL;
        return 0;
    }

    node_queue *aux = *queue;
    node_queue *aux1 = (*queue)->next;
    while(1) {
        free(aux->data);
        free(aux);
        aux = aux1;
        aux1 = aux1->next;
        if(aux1 == *queue) {
            free(aux->data);
            free(aux);
            *queue = NULL;
            return 0;
        }
    }
}

int is_empty(node_queue **queue){
    if(queue == NULL) return -1;
    if(*queue == NULL) return 0;
    else return 1;
}

void print_queue(node_queue **queue){
    if(queue == NULL || *queue == NULL) return;

    node_queue *aux = (*queue)->next;
    do {
        printf("key: %d\n", aux->key);
        aux = aux->next;
    } while(aux != (*queue)->next);
}
/*
retorna 0 se os vetores forem iguais
-1 se contrario ou nao existirem
*/
static int compare_array(int *array1, int *array2, int size) {
    if(array1 == NULL || array2 == NULL || size < 1) return -1;

    int i=0;
    for(i=0; i<size; i++) {
        if(array1[i] != array2[i]) return -1;
    }
    return 0;
}

/*
 * 0  = data found
 * -1 = data not found
*/
int search(node_queue **queue, int *data, int size) {
    if(queue == NULL || *queue == NULL || data == NULL) return -1;

    node_queue *aux = *queue;
    do {
        if(compare_array(data, aux->data, size) == 0) return 0;
        aux = aux->next;
    } while(aux != *queue);
    return -1;
}

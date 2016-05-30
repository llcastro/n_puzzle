#include <stdio.h>
#include <stdlib.h>

typedef struct heap_node {
    int key; // sum of manhattan distance
    void *data; // array of matrix
    struct heap_node *parent;
    struct heap_node *left;
    struct heap_node *right;
} heap_node;

// imprime a chave de cada no na arvore em Pre Ordem.
void pre_order(heap_node *root);

// recebe um no qualquer de uma arvore e busca a raiz.
heap_node *search_root(heap_node *root);

// busca o menor no a esquerda do no recebido.
heap_node *search_min_left(heap_node **root);

// busca o maior no a direita do no recebido.
heap_node *search_max_right(heap_node **leaf);

// procedimento heapify bottom up.
void heapify_bottom_up(heap_node **leaf);

// insere um no em um heap binario.
// funcao principal.
// necessario salvar o valor de retorno no heap
heap_node *insert_node(heap_node **leaf, int k, int *data);

// procedimento heapify top down.
void heapify_top_down(heap_node **root);

// busca o antecessor de um no no heap.
heap_node *predecessor(heap_node **leaf);

// remove um no leaf recebido.
// procedimento secundario.
// void remove_node(heap_node **leaf);

// extrai menor valor do heap.
// retorna o id do menor valor no heap
int *extract_min(heap_node **leaf);

// libera a arvore inteira
void free_priority_queue(heap_node *root);

/*
retorna 0 se data ja existir no heap
-1 caso contrario ou dados inválidos
*/
int search_data(heap_node *root, int *data, int key, int size);

/*
---- Para inserir no heap, por bottom up --------
if(é nulo) insere
if(nao tem filho && pai é nulo) insere a esquerda
if(nao tem filho && tem pai) vai pro pai
if(tem um filho na esquerda && nao tem filho na direita) insere a direita
if(tem dois filhos && tem pai) vai pro pai
if(tem dois filhos && nao tem pai && vim da direita) insere no menor da esquerda
if(tem dois filhos && vim da esquerda) insere no menor da direita

*/

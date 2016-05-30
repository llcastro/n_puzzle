#include <stdio.h>
#include <stdlib.h>
#include "binary_heap.h"

void pre_order(heap_node *root) {
    if(root != NULL) {
		printf("(data:%d-key:%d) ", ((int*) root->data)[0], root->key);
		pre_order(root->left);
		pre_order(root->right);
	}
}

heap_node *search_root(heap_node *root) {
    if(root) {
        if(root->parent) {
            return search_root(root->parent);
        } else {
            return root;
        }
    }
    return NULL;
}

heap_node *search_min_left(heap_node **root) {
    if(root == NULL || *root == NULL) return NULL;

    if((*root)->left == NULL) return *root;
    return search_min_left(&((*root)->left));
}

heap_node *search_max_right(heap_node **leaf) {
    if(leaf == NULL || *leaf == NULL) return NULL;

    if((*leaf)->right == NULL) return *leaf;
    return search_max_right(&((*leaf)->right));
}
// k = key
static heap_node *_insert_node(heap_node **child, int k, int *data) {
    if(child == NULL || data == NULL) return NULL;

    if(*child == NULL) {
        *child = (heap_node*)malloc(sizeof(heap_node));
        if(*child == NULL) return NULL;
        (*child)->left = NULL;
        (*child)->right = NULL;
        (*child)->parent = NULL;
        (*child)->key = k;
        (*child)->data = data;
        return *child;
    }

    if((*child)->left == NULL && (*child)->right == NULL && (*child)->parent == NULL) {
        (*child)->left = (heap_node*)malloc(sizeof(heap_node));
        if((*child)->left == NULL) return NULL;
        (*child)->left->parent = *child;
        (*child)->left->left = NULL;
        (*child)->left->right = NULL;
        (*child)->left->key = k;
        (*child)->left->data = data;
        *child = (*child)->left;
        return *child;
    }

    heap_node *path = NULL;
    while(1) {
        if((*child)->left == NULL && (*child)->right == NULL && (*child)->parent) {
            path = *child;
            *child = (*child)->parent;
        } else if((*child)->left && (*child)->right == NULL) {
            (*child)->right = (heap_node*)malloc(sizeof(heap_node));
            if((*child)->right == NULL) return NULL;
            (*child)->right->parent = *child;
            (*child)->right->left = NULL;
            (*child)->right->right = NULL;
            (*child)->right->key = k;
            (*child)->right->data = data;
            *child = (*child)->right;
            return *child;
        } else if((*child)->left && (*child)->right && (*child)->parent == NULL && (*child)->right == path) {
            heap_node *aux = search_min_left(child);
            if(aux == NULL) return NULL;
            aux->left = (heap_node*)malloc(sizeof(heap_node));
            if(aux->left == NULL) return NULL;
            aux->left->left = NULL;
            aux->left->right = NULL;
            aux->left->parent = aux;
            aux->left->key = k;
            aux->left->data = data;
            aux = aux->left;
            return aux;
        } else if((*child)->left && (*child)->right && (*child)->left == path) {
            heap_node *aux = search_min_left(&((*child)->right));
            if(aux == NULL) return NULL;
            aux->left = (heap_node*)malloc(sizeof(heap_node));
            if(aux->left == NULL) return NULL;
            aux->left->left = NULL;
            aux->left->right = NULL;
            aux->left->parent = aux;
            aux->left->key = k;
            aux->left->data = data;
            aux = aux->left;
            return aux;
        } else if((*child)->left && (*child)->right && (*child)->parent) {
            path = *child;
            *child = (*child)->parent;
        } else {
            return NULL;
        }
    }
}

static void swap(heap_node *x, heap_node *y) {
    if(x == NULL || y == NULL) return;

    int i = x->key, *j = x->data;
    x->key = y->key;
    x->data = y->data;
    y->key = i;
    y->data = j;
}

void heapify_bottom_up(heap_node **leaf) {
    if(leaf == NULL || *leaf == NULL) return;

    heap_node *child = *leaf;

    while(child->parent) {
        if(child->parent->left == child) {
            if(child->parent->right) {
                if(child->parent->right->key < child->key) {
                    if(child->parent->right->key < child->parent->key) {
                        swap(child->parent->right, child->parent);
                        child = child->parent;
                    } else {
                        return;
                    }
                } else {
                    if(child->key < child->parent->key) {
                        swap(child, child->parent);
                        child = child->parent;
                    } else {
                        return;
                    }
                }
            } else {
                if(child->key < child->parent->key) {
                    swap(child, child->parent);
                    child = child->parent;
                } else {
                    return;
                }
            }
        } else {
            if(child->key < child->parent->left->key) {
                if(child->key < child->parent->key) {
                    swap(child, child->parent);
                    child = child->parent;
                } else {
                    return;
                }
            } else {
                if(child->parent->left->key < child->parent->key) {
                    swap(child->parent->left, child->parent);
                    child = child->parent;
                } else {
                    return;
                }
            }
        }
    }
}
// k = key
heap_node *insert_node(heap_node **leaf, int k, int *data) {
    if(leaf == NULL) return NULL;
    heap_node *aux = _insert_node(leaf, k, data);
    if(aux == NULL) return NULL;
    heapify_bottom_up(&aux);
    return aux;
}

void heapify_top_down(heap_node **root) {
    if(root == NULL || *root == NULL) return;

    heap_node *no = *root;

    while(1) {
        if(no->left) {
            if(no->right) {
                if(no->left->key < no->right->key) {
                    if(no->left->key < no->key) {
                        swap(no, no->left);
                        no = no->left;
                    } else {
                        return;
                    }
                } else {
                    if(no->right->key < no->key) {
                        swap(no, no->right);
                        no = no->right;
                    } else {
                        return;
                    }
                }
            } else {
                if(no->left->key < no->key) {
                    swap(no->left, no);
                    no = no->left;
                } else {
                    return;
                }
            }
        } else {
            return;
        }
    }
}

heap_node *predecessor(heap_node **leaf) {
    if(leaf == NULL || *leaf == NULL) return NULL;

    heap_node *no = *leaf;

    while(1) {
        if(no == (no)->parent->left) {
            if((no)->parent->parent == NULL) return search_max_right(&((no)->parent));
            no = (no)->parent;
        } else {
            return search_max_right(&((no)->parent->left));
        }
    }
}

static void remove_node(heap_node **leaf) {
    if(leaf == NULL || *leaf == NULL) return;

    free(*leaf);
    *leaf = NULL;
}
/*
retorna o vetor que forma a matriz
*/
int *extract_min(heap_node **leaf) {
    if(leaf == NULL || *leaf == NULL) return NULL;

    heap_node *root = search_root(*leaf);
    if(root == NULL) return NULL;
    int *k = root->data;
    if(root == *leaf) {
        remove_node(leaf);
        return k;
    }
    heap_node *aux = predecessor(leaf);
    if(aux == NULL) return NULL;
    root->key = (*leaf)->key;
    root->data = (*leaf)->data;

    if((*leaf)->parent != NULL) {
        if((*leaf)->parent->left == *leaf)
            remove_node(&(*leaf)->parent->left);
        else
            remove_node(&(*leaf)->parent->right);
    }

    *leaf = aux;
    heapify_top_down(&root);

    return k;
}

void free_priority_queue(heap_node *root) {
	if(root != NULL) {
		free_priority_queue(root->left);
		free_priority_queue(root->right);
		free(root->data);
        free(root);
	}
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
retorna 0 se data ja existir no heap
-1 caso contrario ou dados inválidos
*/
int search_data(heap_node *root, int *data, int key, int size) {
    if(data == NULL || size < 1) return -1;
    if(root != NULL) {
        if(key == root->key) {
            return compare_array(data, root->data, size);
        }
		int j = search_data(root->left, data, key, size);
        if(j == 0) return 0;
		int k = search_data(root->right, data, key, size);
        if(k == 0) return 0;
        if(root->parent == NULL) return -1;
	} else {
        return 1;
    }
}

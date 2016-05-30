#include <stdio.h>
#include <stdlib.h>
#include "../binary_heap/binary_heap.h"
#include "../queue/queue.h"

/*
cria uma matriz dinamica na memoria
*/
int **create_matrix(int dimension) {
    if(dimension < 2) return NULL;

    int **vector = (int **)calloc(dimension, sizeof(int*));
    if(vector == NULL) return NULL;

    int i=0;
    for (i=0; i<dimension; i++) {
        vector[i] = (int *)calloc(dimension, sizeof(int));
        if(vector[i] == NULL) {
            printf("ERROR\n");
            return NULL;
        }
    }

    return vector;
}
/*
libera o espaço alocado para a matrix
*/
void free_matrix(int **matrix, int dimension) {
    if(matrix == NULL || dimension < 2) return;

    int i=0;
    for (i=0; i<dimension; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
/*
monta a matriz conforme o arquivo recebido por file
*/
int **mount_board(char *file) {
    if(file == NULL) return NULL;

    FILE *fp;
    if ((fp = fopen(file, "r")) == 0) return NULL;

    int dimension=0;
    fscanf(fp, "%d\n", &dimension);
    if(dimension < 2) return NULL;

    int **matrix = create_matrix(dimension);
    if(matrix == NULL) return NULL;

    int i=0, j=0, k=0;
    while(fscanf(fp, "%d", &i) != EOF) {
        matrix[k][j] = i;
        j++;
        if(j >= dimension) {
            j=0;
            k++;
        }
    }

    fclose(fp);

    return matrix;
}

void print_board(int **matrix, int dimension) {
    if(matrix == NULL || *matrix == NULL) return;
    if(dimension < 2) return;

    int i, j;
    for (i = 0; i < dimension; i++) {
        for (j = 0; j < dimension; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}
/*
busca pela coordenada (x, y) de number na matrix
armazena a coordenada em x e y
*/
void search_coordinate(int **matrix, int number, int *x, int *y, int dimension) {
    if(matrix == NULL || *matrix == NULL || x == NULL || y == NULL || dimension < 2) return;

    for (*x = 0; *x < dimension; (*x)++) {
        for (*y = 0; *y < dimension; (*y)++) {
            if(matrix[*x][*y] == number) return;
        }
    }
}
/*
calcula a distancia de manhattan de number entre initial_state e final_state
retorna o valor da distancia;
*/
int calculate_manhattan_distance(int **initial_state, int **final_state, int number, int dimension) {
    if(initial_state == NULL || final_state == NULL || dimension < 2) return -1;
    if(*initial_state == NULL || *final_state == NULL) return -1;

    int x=0, y=0, x1=0, y1=0, k=0, l=0;
    search_coordinate(initial_state, number, &x, &y, dimension);
    search_coordinate(final_state, number, &x1, &y1, dimension);

    if(x1 > x) {
        k = x1-x;
    } else {
        k = x-x1;
    }
    if(y1 > y) {
        l = y1-y;
    } else {
        l = y-y1;
    }

    return (k+l);
}

/*
calcula a distancia de manhattan entre initial_state e final_state
retorna um vetor de inteiros com as distancias
*/
int *calculate_manhattan_distances(int **initial_state, int **final_state, int dimension) {
    if(initial_state == NULL || final_state == NULL || dimension < 2) return NULL;
    if(*initial_state == NULL || *final_state == NULL) return NULL;

    int i=0, j=0;
    int *v = (int*)calloc(dimension*dimension, sizeof(int));
    if(v == NULL) return NULL;

    for (i=0; i<dimension; i++) {
        for (j=0; j<dimension; j++) {
            v[initial_state[i][j]] = calculate_manhattan_distance(initial_state, final_state, initial_state[i][j], dimension);
            if(v[initial_state[i][j]] < 0) return NULL;
        }
    }

    return v;
}

/*
transforma a matrix em um array
para ser armazenado na fila de prioridades
retorna o array da matrix
*/
int *matrix_to_array(int **matrix, int dimension) {
    if(matrix == NULL || *matrix == NULL || dimension < 2) return NULL;

    int *v = (int*)calloc(dimension*dimension, sizeof(int));
    if(v == NULL) return NULL;

    int i=0, j=0, k=0;
    for(i=0; i<dimension; i++) {
        for(j=0; j<dimension; j++) {
            v[k] = matrix[i][j];
            k++;
        }
    }
    return v;
}

/*
calcula a soma das distancias de manhattan contidas em distances
size = tamanho do array distances
retorna a soma
*/
int manhattan_distances_sum(int *distances, int size) {
    if(distances == NULL || size < 1) return -1;

    int i=0, sum=0;
    for(i=0; i<size; i++) {
        sum += distances[i];
    }
    return sum;
}
/*
calcula a soma das distancias de hamming contidas em distances
size = tamanho do array distances
retorna a soma
*/
int hamming_heuristic_sum(int *distances, int size) {
    if(distances == NULL || size < 1) return -1;

    int i=0, sum=0;
    for(i=0; i<size; i++) {
        if(distances[i] != 0) {
            sum += 1;
        }
    }
    return sum;
}
/*
adiciona a matriz em forma de array na priority queue
checa se ja nao existe a mesma matriz na mesma
*/
void add_priority_queue(heap_node **leaf, int key, int *array, int size) {
    if(leaf == NULL || array == NULL || size < 1) return;

    int i = search_data(search_root(*leaf), array, key, size);
    if(i == 0) return; // array exist in priority queue

    *leaf = (heap_node*) insert_node(leaf, key, array);
}
/*
troca 2 valores da matrix
0 se a troca ocorrer
-1 caso contrario ou dados invalidos
*/
int swap(int **matrix, int x0, int y0, int x1, int y1, int dimension) {
    if(matrix == NULL || *matrix == NULL || dimension < 2) return -1;
    if(x0 < 0 || x0 >= dimension || y0 < 0 || y0 >= dimension) return -1;
    if(x1 < 0 || x1 >= dimension || y1 < 0 || y1 >= dimension) return -1;

    int i=matrix[x0][y0];
    matrix[x0][y0] = matrix[x1][y1];
    matrix[x1][y1] = i;

    return 0;
}
/*
gera os filhos do estado atual
calcula a distancia de manhattan
transforma a matriz permutada em vetor
adiciona na fila de prioridades (open list) que so adiciona se nao existir a mesma
*/
void add_clildren(int **initial_state, int **final_state, heap_node **leaf, node_queue **queue, int dimension) {
    if(initial_state == NULL || final_state == NULL || dimension < 2) return;
    if(*initial_state == NULL || *final_state == NULL) return;
    if(leaf == NULL || queue == NULL) return;

    int x=0, y=0;
    search_coordinate(initial_state, 0, &x, &y, dimension);

    if(swap(initial_state, x, y, x-1, y, dimension) == 0) {
        int *distances = calculate_manhattan_distances(initial_state, final_state, dimension);
        if(distances == NULL) return;

        int sum = manhattan_distances_sum(distances, dimension*dimension);
        sum += hamming_heuristic_sum(distances, dimension*dimension);
        free(distances);

        int *array = matrix_to_array(initial_state, dimension);
        if(array == NULL) return;

        int i = search(queue, array, dimension*dimension);
        if(i != 0) {
            add_priority_queue(leaf, sum, array, dimension*dimension);
        } else {
            free(array);
        }
        swap(initial_state, x, y, x-1, y, dimension);
    }
    if(swap(initial_state, x, y, x+1, y, dimension) == 0) {
        int *distances = calculate_manhattan_distances(initial_state, final_state, dimension);
        if(distances == NULL) return;

        int sum = manhattan_distances_sum(distances, dimension*dimension);
        sum += hamming_heuristic_sum(distances, dimension*dimension);
        free(distances);

        int *array = matrix_to_array(initial_state, dimension);
        if(array == NULL) return;

        int i = search(queue, array, dimension*dimension);
        if(i != 0) {
            add_priority_queue(leaf, sum, array, dimension*dimension);
        } else {
            free(array);
        }
        swap(initial_state, x, y, x+1, y, dimension);
    }
    if(swap(initial_state, x, y, x, y-1, dimension) == 0) {
        int *distances = calculate_manhattan_distances(initial_state, final_state, dimension);
        if(distances == NULL) return;

        int sum = manhattan_distances_sum(distances, dimension*dimension);
        sum += hamming_heuristic_sum(distances, dimension*dimension);
        free(distances);

        int *array = matrix_to_array(initial_state, dimension);
        if(array == NULL) return;

        int i = search(queue, array, dimension*dimension);
        if(i != 0) {
            add_priority_queue(leaf, sum, array, dimension*dimension);
        } else {
            free(array);
        }
        swap(initial_state, x, y, x, y-1, dimension);
    }
    if(swap(initial_state, x, y, x, y+1, dimension) == 0) {
        int *distances = calculate_manhattan_distances(initial_state, final_state, dimension);
        if(distances == NULL) return;

        int sum = manhattan_distances_sum(distances, dimension*dimension);
        sum += hamming_heuristic_sum(distances, dimension*dimension);
        free(distances);

        int *array = matrix_to_array(initial_state, dimension);
        if(array == NULL) return;

        int i = search(queue, array, dimension*dimension);
        if(i != 0) {
            add_priority_queue(leaf, sum, array, dimension*dimension);
        } else {
            free(array);
        }
        swap(initial_state, x, y, x, y+1, dimension);
    }
}
/*
cria uma matriz a partir de um array
retorna a matriz ou nulo caso ocorra erro
*/
int **array_to_matrix(int *array, int dimension) {
    if(array == NULL || dimension < 2) return NULL;

    int **matrix = create_matrix(dimension);
    if(matrix == NULL) {
        free(array);
        return NULL;
    }

    int i=0, j=0, k=0;
    for(i=0; i<dimension; i++) {
        for(j=0; j<dimension; j++) {
            matrix[i][j] = array[k];
            k++;
        }
    }
    free(array);
    return matrix;
}

/*
verifica se objetivo foi alcancado
0 = objetivo alcancado
-1 = objetivo nao alcancado ou dados invalidos
*/
int check(int **initial_state, int **final_state, int dimension) {
    if(initial_state == NULL || final_state == NULL || dimension < 2) return -1;
    if(*initial_state == NULL || *final_state == NULL) return -1;

    int *v = calculate_manhattan_distances(initial_state, final_state, dimension);
    if(v == NULL) return -1;

    int i = manhattan_distances_sum(v, dimension*dimension);
    free(v);
    if(i == 0) return 0;
    else return -1;
}
/*
adiciona a matriz em closed set checando se ja nao existe
matrix nao liberado pois eh usado por outras funcoes
*/
int add_closed_set(int **matrix, node_queue **queue, int dimension) {
    if(matrix == NULL || *matrix == NULL || queue == NULL) return -1;
    if(dimension < 2) return -1;

    int *array = matrix_to_array(matrix, dimension);
    if(array == NULL) return -1;

    int i = search(queue, array, dimension*dimension);
    if(i == 0) {
        free(array);
        return -1; // already exist
    } else {
        add(queue, array);
        return 0;
    }
}
/*
checa se o array estraido ja existe em closed set(queue)
caso exista, retira outra de open set(leaf)
caso contrario, retorna ela em formato de matriz
NULL em caso de erro
*/
int **extract_min_checked(heap_node **leaf, node_queue **queue, int dimension) {
    if(leaf == NULL || queue == NULL || dimension < 2) return NULL;

    if(*queue == NULL) {
        int *array = (int*) extract_min(leaf);
        if(array == NULL) return NULL;
        int **x = array_to_matrix(array, dimension);
        return x;
    } else {
        int i=1;
        do {
            int *array = (int*) extract_min(leaf);
            if(array == NULL) return NULL;

            i = search(queue, array, dimension*dimension);
            if(i != 0) {
                int **x = array_to_matrix(array, dimension);
                return x;
            }
            free(array);
        } while(i == 0); // 0 = ja existente
        return NULL;
    }
}

/*
resolve o puzzle
leaf = open list
queue = closed list
0 = deu certo
-1 = esgotou todas as possibilidades ou entrada invalida
*/
int solve_puzzle(int **initial_state, int **final_state, heap_node **leaf, node_queue **queue, int dimension) {
    if(initial_state == NULL || final_state == NULL || dimension < 2) return -1;
    if(*initial_state == NULL || *final_state == NULL) return -1;
    if(leaf == NULL || queue == NULL) return -1;

    int *distances = calculate_manhattan_distances(initial_state, final_state, dimension);
    if(distances == NULL) return -1;
    int sum = manhattan_distances_sum(distances, dimension*dimension);
    sum += hamming_heuristic_sum(distances, dimension*dimension);
    free(distances);
    int *array = matrix_to_array(initial_state, dimension);
    if(array == NULL) return -1;
    add_priority_queue(leaf, sum, array, dimension*dimension);

    int *array_aux = (int*) extract_min(leaf);
    if(array_aux == NULL) return -1;
    int **x = array_to_matrix(array_aux, dimension);

    int i=0;
    do {
        add_clildren(x, final_state, leaf, queue, dimension);

        add_closed_set(x, queue, dimension); // rever necessidade da funcao

        if(i > 20000) {
            printf("ERROR\n");
            return -1;
        }

        if(check(x, final_state, dimension) == 0) {
            free_matrix(x, dimension);
            printf("necessary steps: %d\n", i);
            return 0;
        }
        free_matrix(x, dimension);
        x = extract_min_checked(leaf, queue, dimension);
        i++;
    } while(*leaf != NULL);
    free_matrix(x, dimension);

    return -1;
}
/*
TODO tentar diminuir o numero de passos
*/
int main(void) {

    int dimension=3;
    int **initial = mount_board("./initial_state.txt");
    if(initial == NULL) return -1;
    int **final = mount_board("./final_state.txt");
    if(final == NULL) return -1;

    heap_node *node = NULL;
    node_queue *queue = NULL;

    solve_puzzle(initial, final, &node, &queue, dimension);

    free_matrix(initial, dimension);
    free_matrix(final, dimension);
    free_priority_queue(search_root(node));
    free_queue(&queue);

    printf("ok\n");

    return 0;
}

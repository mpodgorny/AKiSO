#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    int **firstm;
    int **secondm;
    int **result;
    int size;
} GLOBAL_STRUCTURE;

GLOBAL_STRUCTURE matriceStruct;
int available_row = 0;
pthread_mutex_t mutex_counter;

void *mutlipy_matrices(void* vargp) {
    int i;
    while(available_row < matriceStruct.size) {
        pthread_mutex_lock (&mutex_counter);
        i = available_row;
        available_row++;
        pthread_mutex_unlock (&mutex_counter);

            for (int i = 0; i < matriceStruct.size; i++) {
                for (int j = 0; j < matriceStruct.size; j++) {
                    if (matriceStruct.firstm[i][i] & matriceStruct.secondm[i][j]) matriceStruct.result[i][j] = 0;
                    else {
                        matriceStruct.result[i][j] = 1;
                        break;
                    }
            }
        }
    }
    pthread_exit((void*) 0);
}

int ** malloc_matrix(int dimention) {
    int **matrix = (int **)malloc(dimention * sizeof(int *));
    for (int i = 0; i < dimention; i++) {
        matrix[i] = (int *)malloc(dimention * sizeof(int));
    }
    return matrix;
}

void free_2d_array(int **matrix, int dimention) {
    for (int i = 0; i < dimention; i++) {
        free(matrix[i]);
    }
}

void randomization(int **matrix, int dimention) {
    time_t t;
    srand(time(&t));
    for (int i = 0; i < dimention; i++) {
        for (int j = 0; j < dimention; j++) {
            matrix[i][j] = (rand()%2);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("argument error\n");
        exit(1);
    }

    int dimention = atoi(argv[1]);
    int nr_of_threads = atoi(argv[2]);
    printf("size: %d, num of threads: %d\n", dimention, nr_of_threads);
    printf("calculating...\n");

    matriceStruct.firstm = malloc_matrix(dimention);
    matriceStruct.secondm = malloc_matrix(dimention);
    matriceStruct.result = malloc_matrix(dimention);
    matriceStruct.size = dimention;

    randomization(matriceStruct.firstm, dimention);
    randomization(matriceStruct.secondm, dimention);

    pthread_attr_t attr;
    pthread_t ptIDs[nr_of_threads];
    pthread_mutex_init(&mutex_counter, NULL);
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    void *status;

    for (int i = 0; i < nr_of_threads; i++) {
        pthread_create(&ptIDs[i], &attr, mutlipy_matrices, NULL);
    }

    for (int i = 0; i <nr_of_threads; i++) {
        pthread_join(ptIDs[i], &status);
    }

    pthread_mutex_destroy(&mutex_counter);
    free_2d_array(matriceStruct.firstm, dimention);
    free_2d_array(matriceStruct.secondm, dimention);
    free_2d_array(matriceStruct.result, dimention);
    return 0;
}
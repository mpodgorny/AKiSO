#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void cache_otp_algo(int **arr1, int **arr2, int **res, int size, int trans) {
    int i, j, k,ii,ib,acc00,acc01,acc10,acc11;
    ib=4;
    if(trans){
        for (ii = 0; ii < size; ii += ib) {
            for (j = 0; j < size; j += 2) {
                for (i = ii; i < ii + ib; i += 2) {
                    acc00 = acc01 = acc10 = acc11 = 0;
                    for (k = 0; k < size; k++) {
                        acc00 += arr1[j][k + 0] * arr2[i + 0][k];
                        acc01 += arr1[j][k + 1] * arr2[i + 0][k];
                        acc10 += arr1[j][k + 0] * arr2[i + 1][k];
                        acc11 += arr1[j][k + 1] * arr2[i + 1][k];
                    }
                    res[i + 0][j + 0] = acc00;
                    res[i + 0][j + 1] = acc01;
                    res[i + 1][j + 0] = acc10;
                    res[i + 1][j + 1] = acc11;
                }
            }
        }
    }else {
        for (ii = 0; ii < size; ii += ib) {
            for (j = 0; j < size; j += 2) {
                for (i = ii; i < ii + ib; i += 2) {
                    acc00 = acc01 = acc10 = acc11 = 0;
                    for (k = 0; k < size; k++) {
                        acc00 += arr1[k][j + 0] * arr2[i + 0][k];
                        acc01 += arr1[k][j + 1] * arr2[i + 0][k];
                        acc10 += arr1[k][j + 0] * arr2[i + 1][k];
                        acc11 += arr1[k][j + 1] * arr2[i + 1][k];
                    }
                    res[i + 0][j + 0] = acc00;
                    res[i + 0][j + 1] = acc01;
                    res[i + 1][j + 0] = acc10;
                    res[i + 1][j + 1] = acc11;
                }
            }
        }
    }
}

void normal_mutliplying(int **arr1, int **arr2, int **res, int size, int transposed) {
    for(int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                if(transposed) res[i][j] += arr1[i][k] * arr2[j][k];
                else           res[i][j] += arr1[i][k] * arr2[k][j];

            }
        }
    }
}

void randomize_values(int ***arr, int size) {
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            (*arr)[i][j] = rand();
        }
    }
}

void calloc_matrix(int ***arr, int size) {

    *arr = calloc(size, sizeof(int*)); //callocujemy i zerujemy liczbe kolumn = size

    for (int i = 0; i < size; i++) {
        (*arr)[i] = (int *) calloc(size, sizeof(int)); //callocujemy poszczegolne wiersze
    }
}

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("1.:size 2.:(1 or 0) transposition 3.:(1 or 0) cache optimization algorithm\n");
        return -1;
    }
    int size = (int) strtol(argv[1], (char **)NULL, 10);
    int transposition = (int) strtol(argv[2], (char **)NULL, 10);
    int bool_cache_opt = (int) strtol(argv[3], (char **)NULL, 10);
    clock_t start, end;
    double cpu_time_used;

    int **A, **B, **tB, **R;
    calloc_matrix(&A, size);
    randomize_values(&A, size);

    calloc_matrix(&B, size);
    randomize_values(&B, size);

    calloc_matrix(&tB, size);

    calloc_matrix(&R, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            tB[j][i] = B[i][j];
        }
    }

    printf("Processing with ");

    start=clock();

    if(bool_cache_opt == 1)  {
        printf("cache optimization and ");
        if (transposition == 1) {
            printf("transpositon...\n");
            cache_otp_algo(A, tB, R, size, transposition);
        } else {
            printf("no transpositon...\n");
            cache_otp_algo(A, B, R, size, transposition);
        }
    }else if (transposition == 1) {
        printf("no cache optimization and transposition...\n");
        normal_mutliplying(A,tB,R,size, transposition);
    } else {
        printf("no cache optimization and no transposition...\n");
        normal_mutliplying(A,B,R,size, transposition);
    }
    end = clock();
    cpu_time_used= ((double) (end - start))/1000000;
    printf("Finished. Execution time: %fs\n", cpu_time_used);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "read_matrix.h"
#include "configuration_matrix.h"
#include "struct.h"


// random choice of clusters
void configurationMatrix(int nclusters, int nrows , CSR *csrMatrix){

    
    // calloc because configuration Matrix has zero elements
    int **configurationMatrix = (int **)calloc(nrows, sizeof(int*));

    for (int i = 0; i < nrows; i++) {
        configurationMatrix[i] = (int *)calloc(nclusters, sizeof(int));
    }

    int row_index = 0;

    for (int i = 0; i < nrows; i++) {
        int nonZeroCol = rand() % nclusters;
        //configurationMatrix[i][nonZeroCol] = 1;
        csrMatrix->cols[row_index] = nonZeroCol;
        csrMatrix->rows[row_index + 1] = row_index + 1; // Increment the row_index for the next iteration
        row_index++;
    }

    // Print the matrix
    // printf("\n Configuration Matrix:\n");

    // for (int i = 0; i < nrows; i++) {
    //     for (int j = 0; j < nclusters; j++) {
    //         printf("%d ", configurationMatrix[i][j]);
    //     }
    //     printf("\n");
    // }

    printf("CSR MATRIX FOR MATRIX Ω:\n");
    printCsrMatrix(*csrMatrix, nrows);

    
}


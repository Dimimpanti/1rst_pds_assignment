#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "read_matrix.h"
#include "configuration_matrix.h"
#include "struct.h"


// random choice of clusters
void configurationMatrix(int nclusters, int nrows , CSR *csrMatrix){

    
    // calloc for csrMatrix because it has to be initialized with 0
    int **configurationMatrix = (int **)calloc(nrows, sizeof(int*));

    for (int i = 0; i < nrows; i++) {
        configurationMatrix[i] = (int *)calloc(nclusters, sizeof(int));
    }

    int row_index = 0; // the index of the row we are looking at

    for (int i = 0; i < nrows; i++) {
        int nonZeroCol = rand() % nclusters;
        csrMatrix->cols[row_index] = nonZeroCol; 
        csrMatrix->rows[row_index + 1] = row_index + 1; // Increment the row_index for the next iteration
        row_index++;
    }

    printf("CSR MATRIX FOR MATRIX Ω:\n");
    printCsrMatrix(csrMatrix);

    
}


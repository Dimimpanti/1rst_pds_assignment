#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "read_matrix.h"
#include "configuration_matrix.h"


//random choice of clusters
void configurationMatrix(int clusters, int nrows, FILE *initialFile) {
    
    rewind(initialFile);

    skipHeader(initialFile);

    int *configurationMatrix = (int *)malloc(nrows * clusters * sizeof(int));

    for (int i = 0; i < nrows; i++) {
        int nonZeroCol = rand() % clusters;

        for (int j = 0; j < clusters; j++) {
            MatrΩ[i * clusters + j] = 0;
        }

        MatrΩ[] = 1;
    }

    // Print the matrix
    printf("\n MatrixΩ:\n");
    
 
}
 




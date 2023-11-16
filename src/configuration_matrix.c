#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "read_matrix.h"
#include "configuration_matrix.h"


//random choice of clusters
void configurationMatrix(int nclusters, int nrows) {
    
    int **configurationMatrix = (int **)malloc(nrows * sizeof(int*));
    
    for (int i = 0; i < nrows; i++) {
        configurationMatrix[i] = (int *)malloc(nclusters * sizeof(int));
    }

    for (int i = 0; i < nrows; i++) {
        int nonZeroCol = rand() % nclusters;

        for (int j = 0; j < nclusters; j++) {

            
        }

      
    }

    // Print the matrix
    printf("\n MatrixΩ:\n");
    
 
}
 

// void printMatrixΩ(int *matrix, int rows, int cols) {
//     for (int i = 0; i < rows; i++) {
//         for (int j = 0; j < cols; j++) {
//             printf("%d ", matrix[i * cols + j]);
//         }
//         printf("\n");
//     }
// }



// void CsrMatrixΩ(int *MatrΩ, int nrows, int clusters) {
//     // Count the number of non-zero elements (nnz)
//     int nnz = 0;
//     for (int i = 0; i < nrows * clusters; i++) {
//         if (MatrΩ[i] != 0) {
//             nnz++;
//         }
//     }

//     // Allocate memory for CSR arrays
//     int *csrRows = (int *)calloc(nrows, sizeof(int));
//     int *csrCols = (int *)calloc(nnz , sizeof(int));

//     // Fill CSR arrays

//     int row_index = 0; //iritates trough the matrix
//     for (int i = 0; i < nrows; i++) {
//         for (int j = 0; j < clusters; j++) {
//             if (MatrΩ[i] != 0) {
//                 csrCols[row_index] = j;
//                 csrRows[i + 1]++;//only one non zero value per row 
//                 row_index++;
//             }
//         }
//         csrRows[i+1] += csrRows[i];
//     }


//     // Print the CSR matrix
//     printf("\nCSR MatrixΩ:\n");
//     printf("Csr Rows:");
//     for (int i = 0; i <= nrows; i++) {
//         printf("%d ", csrRows[i]);
//     }
//     printf("\n");
//     printf("Csr Cols:");
//     for (int i = 0; i < nnz; i++) {
//         printf("%d ", csrCols[i]);
//     }
//     printf("\n");}



// //random choice of clusters
// void MatrixΩ(int clusters, int nrows, FILE *initialFile) {
//     rewind(initialFile);
//     skipHeader(initialFile);

//     int *MatrΩ = (int *)malloc(nrows * clusters * sizeof(int));

//     for (int i = 0; i < nrows; i++) {
//         int nonZeroCol = rand() % clusters;

//         for (int j = 0; j < clusters; j++) {
//             MatrΩ[i * clusters + j] = 0;
//         }

//         MatrΩ[i * clusters + nonZeroCol] = 1;
//     }

//     // Print the matrix
//     printf("\n MatrixΩ:\n");
//     printMatrixΩ(MatrΩ, nrows, clusters);
//     CsrMatrixΩ(MatrΩ , nrows , clusters);
 
// }


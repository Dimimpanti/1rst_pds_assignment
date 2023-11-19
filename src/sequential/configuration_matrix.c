#include <stdio.h>
#include <stdlib.h>

#include "../read_matrix.h"
#include "configuration_matrix.h"


/**
 * @brief Generates the transpose of a random configuration matrix in CSR format
 * 
 * @param nclusters  The number of clusters
 * @param csrMatrix  The CSR matrix
 */
void configurationMatrix(int nclusters, CSC *cscMatrix){

    // Check if the number of clusters is greater than the number of nodes in the graph
    if(nclusters > cscMatrix->nz){
        fprintf(stderr, "Error! The number of clusters is greater than the number of nodes in the graph\n");
        exit(1);
    }

    int nodesPerCluster = cscMatrix->nz / nclusters;  // The number of nodes per cluster
    int remainder = cscMatrix->nz % nclusters;        // The remainder of the division of the number of nodes with the number of clusters

    // The configuration matrix has 1 non zero element per row and the value of the element is 1
    for(int i = 0; i< cscMatrix->nz; i++){
        cscMatrix->rows[i] = i;
        cscMatrix->values[i] = 1;
    }

    cscMatrix->cols[0] = 0;

    for (int i = 0; i < cscMatrix->ncols; i++){
        cscMatrix->cols[i + 1] = cscMatrix->cols[i] + nodesPerCluster;
    }
    
    
    cscMatrix->cols[cscMatrix->ncols] += remainder;
    
}





/**
 * @brief Given a CSC matrix, this function creates a CSR matrix that is the transpose of the CSC matrix
 * 
 * @param input   The CSC matrix
 * @param output  The transposed matrix in CSR format
 */
void transposeConfigurationMatrix(CSC *input , CSR *output){
    
    output->nz = input->nz;  // The number of non-zero elements is equal to the number of non-zero elements of the input matrix
    output->nrows = input->ncols;  // The number of rows is equal to the number of columns of the input matrix
    output->ncols = input->nrows;  // The number of columns is equal to the number of rows of the input matrix

    // Allocate memory for the arrays of the output matrix
    output->cols = (int *)malloc(input->nz * sizeof(int));           //TODO : free done
    output->rows = (int *)malloc((input->ncols + 1) * sizeof(int));  //TODO : free done
    output->values = (int *)malloc(input->nz * sizeof(int));         //TODO : free done


    for (int i = 0; i < input->nz; i++) {
        output->cols[i] = input->rows[i];
        output->values[i] = input->values[i];
    }
    

    for (int i = 0; i < input->ncols + 1; i++) {
        output->rows[i] = input->cols[i];
    }
}
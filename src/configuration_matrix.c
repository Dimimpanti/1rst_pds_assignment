#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "read_matrix.h"
#include "configuration_matrix.h"


/**
 * @brief Generates the transpose of a random configuration matrix in CSR format
 * 
 * @param nclusters  The number of clusters
 * @param csrMatrix  The CSR matrix
 */
void configurationMatrix(int nclusters, CSC *cscMatrix){

    if(nclusters > cscMatrix->nz){
        fprintf(stderr, "Error! The number of clusters is greater than the number of nodes in the graph\n");
        exit(1);
    }

    int nodesPerCluster = cscMatrix->nz / nclusters;
    int remainder = cscMatrix->nz % nclusters;

    for(int i = 0; i< cscMatrix->nz; i++){
        cscMatrix->rows[i] = i;
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
    
    output->nz = input->nz;
    output->nrows = input->ncols;
    output->cols = (int *)malloc(input->nz * sizeof(int));           //TODO : free done
    output->rows = (int *)malloc((input->ncols + 1) * sizeof(int));  //TODO : free done
    output->values = (int *)malloc(input->nz * sizeof(int));         //TODO : free done

    // initialize all the values of the matrix to 1
    for(int i = 0; i < output->nz; i++){
        output->values[i] = 1;
    }


    for (int i = 0; i < input->nz; i++) {
        output->cols[i] = input->rows[i];
    }
    

    for (int i = 0; i < input->ncols + 1; i++) {
        output->rows[i] = input->cols[i];
    }
}
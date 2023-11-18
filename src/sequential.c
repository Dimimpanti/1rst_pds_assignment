#include <stdio.h>
#include <stdlib.h>

#include "read_matrix.h"
#include "configuration_matrix.h"
#include "csr_csc_multiplication.h"
#include "structs.h"


int main(){

    FILE *fptr;
    // fptr = fopen ("./../graphs/com_youtube/com_youtube.mtx" , "r");
    fptr = fopen ("./../graphs/test_matrices/small_graph_2.mtx" , "r");
    
    if (fptr == NULL){
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        return 1;
    }

    int nrows, ncols , nz;
    int nclusters = 4;

    readHeader(&nrows, &ncols, &nz, fptr);

    // Create the CSR matrix from the file
    CSC cscMatrix;

    cscMatrix.ncols = ncols;
    cscMatrix.nz = 2 * nz;
    cscMatrix.cols = (int *)malloc((cscMatrix.ncols + 1) * sizeof(int));  //TODO : free done
    cscMatrix.rows = (int *)malloc(cscMatrix.nz * sizeof(int));           //TODO : free done
    cscMatrix.values = (int *)malloc(cscMatrix.nz * sizeof(int));         //TODO : free done

    // initialize all the values of the matrix to 1
    for(int i = 0; i < cscMatrix.nz; i++){
        cscMatrix.values[i] = 1;
    }
    

    createCscMatrix(&cscMatrix, fptr, nz, nrows);
    printCscMatrix(&cscMatrix);


    // Create the configuration matrix
    CSC cscConfigMatrix;
    
    cscConfigMatrix.ncols= nclusters;
    cscConfigMatrix.nz = nrows;
    cscConfigMatrix.cols = (int *)malloc((cscConfigMatrix.ncols + 1) * sizeof(int));  //TODO : free done
    cscConfigMatrix.rows = (int *)malloc(cscConfigMatrix.nz * sizeof(int));           //TODO : free done
    cscConfigMatrix.values = (int *)malloc(cscConfigMatrix.nz * sizeof(int));         //TODO : free done

    // initialize all the values of the matrix to 1
    for(int i = 0; i < cscConfigMatrix.nz; i++){
        cscConfigMatrix.values[i] = 1;
    }
   

    configurationMatrix(nclusters, &cscConfigMatrix);
    printCscMatrix(&cscConfigMatrix);


    // Create the transpose of the configuration matrix
    CSR csrConfigTMatrix;

    transposeConfigurationMatrix(&cscConfigMatrix, &csrConfigTMatrix);
    printCsrMatrix(&csrConfigTMatrix);

    CSR configTProduct;

    // Multiply the transpose of the configuration matrix with the original matrix
    csrCscMultiplication(&csrConfigTMatrix, &cscMatrix, &configTProduct);

    printCsrMatrix(&configTProduct);
    printDenseMatrix(&configTProduct);

    fclose(fptr); 

    free(cscMatrix.cols);
    free(cscMatrix.rows);
    free(cscConfigMatrix.cols);
    free(cscConfigMatrix.rows);
    free(csrConfigTMatrix.cols); 
}
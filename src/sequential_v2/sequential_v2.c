#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "../read_matrix.h"
#include "../configuration_matrix.h"
#include "graph_minor.h"
#include "../structs.h"


/**
 * Calculates the elapse time
 *
 * @param begin the starting timestamp
 * @param end the ending timestamp
 * @return elapsed time in seconds
 */
double measureTime(struct timeval begin, struct timeval end) {
    long seconds;
    long microseconds;
    double elapsed;

    seconds = end.tv_sec - begin.tv_sec;
    microseconds = end.tv_usec - begin.tv_usec;
    elapsed = seconds + microseconds * 1e-6;

    return elapsed;
}


int main(int argc, char *argv[]){

    if (argc != 3) {
        printf("\n\n    Usage: ./sequential <path_to_graph> <path_to_cluster_file>\n\n");
        return 1;
    }

    FILE *fptr;

    printf("\n\n    SEQUENTIAL\n\n");
    printf("    Reading matrix %s ...\n", argv[1]);
    fptr = fopen (argv[1] , "r");
    
    if (fptr == NULL){
        printf("\n\n    Error! opening matrix file\n\n");
        // Program exits if file pointer returns NULL.
        return 1;
    }

    struct timeval begin, end, totalBegin, totalEnd;

    gettimeofday(&begin, NULL);
    gettimeofday(&totalBegin, NULL);

    int nrows, ncols , nz;
    

    int hasValues = readHeader(&nrows, &ncols, &nz, fptr);

    // Create the CSR matrix from the file
    CSR csrMatrix;

    csrMatrix.ncols = ncols;  // The columns of the matrix are read from the file
    csrMatrix.nrows = nrows;  // The rows of the matrix are read from the file
    csrMatrix.nz = 2 * nz;    // The number of non-zero elements is doubled since the matrix is symmetric

    // Allocate memory for the arrays of the CSR matrix
    csrMatrix.rows = (int *)malloc((csrMatrix.nrows + 1) * sizeof(int));  //TODO : free done
    csrMatrix.cols = (int *)malloc(csrMatrix.nz * sizeof(int));           //TODO : free done
    csrMatrix.values = (int *)malloc(csrMatrix.nz * sizeof(int));         //TODO : free done
    

    createCsrMatrix(&csrMatrix, fptr, hasValues);
    // printCsrMatrix(&csrMatrix);

    gettimeofday(&end, NULL);
    printf("    Time for matrix read: %.5f seconds.\n\n", measureTime(begin, end));


    printf("    Creating configuration matrix...\n");
    gettimeofday(&begin, NULL);

    // Create the configuration matrix
    CSR csrConfigMatrix; 

    FILE *fptrConfig;

    fptrConfig = fopen (argv[2] , "r");

    if (fptrConfig == NULL){
        printf("\n\n    Error! opening cluster file\n\n");
        // Program exits if file pointer returns NULL.
        return 1;
    }

    csrConfigurationMatrix(fptrConfig, &csrConfigMatrix, csrMatrix.nrows);
    // printCscMatrix(&cscConfigMatrix);

    gettimeofday(&end, NULL);
    printf("    Time for configuration matrix creation: %.5f seconds.\n\n", measureTime(begin, end));

    CSR csrFinalProduct;

    printf("    Computing graph minor...\n");
    gettimeofday(&begin, NULL);

    computeGraphMinor(&csrMatrix, &csrConfigMatrix, &csrFinalProduct);

    gettimeofday(&end, NULL);
    gettimeofday(&totalEnd, NULL);
    printf("    Time for graph minor computation: %.5f seconds.\n\n", measureTime(begin, end));
    printf("    Total time: %.5f seconds.\n", measureTime(totalBegin, totalEnd));

    // printCsrMatrix(&csrFinalProduct);
    // printDenseCSRMatrix(&csrFinalProduct);

    fclose(fptr);
    fclose(fptrConfig);

    free(csrMatrix.cols);
    free(csrMatrix.rows);
    free(csrConfigMatrix.cols);
    free(csrConfigMatrix.rows);
 
}
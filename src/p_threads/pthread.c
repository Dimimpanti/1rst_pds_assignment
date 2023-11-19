#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "../read_matrix.h"
#include "configuration_matrix_pthread.h"
#include "csr_csc_multiplication_pthread.h"
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


int main(){

    FILE *fptr;
    // fptr = fopen ("./../../graphs/belgium_osm/belgium_osm.mtx" , "r");
    // fptr = fopen ("./../../graphs/mycielskian13/mycielskian13.mtx" , "r");
    // fptr = fopen ("./../../graphs/com_youtube/com_youtube.mtx" , "r");
    // fptr = fopen ("./../../graphs/test_matrices/small_graph_2.mtx" , "r");
    fptr = fopen ("./../../graphs/s3dkq4m2.mtx" , "r");
    
    if (fptr == NULL){
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        return 1;
    }

    struct timeval begin, end, totalBegin, totalEnd;

    printf("Reading matrix... ");
    gettimeofday(&begin, NULL);
    gettimeofday(&totalBegin, NULL);

    int nrows, ncols , nz;
    int nclusters = 1000;

    int hasValues = readHeader(&nrows, &ncols, &nz, fptr);

    // Create the CSR matrix from the file
    CSC cscMatrix;

    cscMatrix.ncols = ncols;  // The columns of the matrix are read from the file
    cscMatrix.nrows = nrows;  // The rows of the matrix are read from the file
    cscMatrix.nz = 2 * nz;    // The number of non-zero elements is doubled since the matrix is symmetric

    // Allocate memory for the arrays of the CSR matrix
    cscMatrix.cols = (int *)malloc((cscMatrix.ncols + 1) * sizeof(int));  //TODO : free done
    cscMatrix.rows = (int *)malloc(cscMatrix.nz * sizeof(int));           //TODO : free done
    cscMatrix.values = (int *)malloc(cscMatrix.nz * sizeof(int));         //TODO : free done
    

    createCscMatrix(&cscMatrix, fptr, hasValues);
    // printCscMatrix(&cscMatrix);

    gettimeofday(&end, NULL);
    printf("Time for matrix read: %.5f seconds.\n", measureTime(begin, end));


    printf("Creating configuration matrix... ");
    gettimeofday(&begin, NULL);

    // Create the configuration matrix
    CSC cscConfigMatrix;
    
    cscConfigMatrix.ncols= nclusters;  // The columns of the matrix are the number of clusters
    cscConfigMatrix.nrows = nrows;     // The rows of the matrix are the number of vertices
    cscConfigMatrix.nz = nrows;        // The number of non-zero elements is equal to the number of vertices

    // Allocate memory for the arrays of the configuration matrix
    cscConfigMatrix.cols = (int *)malloc((cscConfigMatrix.ncols + 1) * sizeof(int));  //TODO : free done
    cscConfigMatrix.rows = (int *)malloc(cscConfigMatrix.nz * sizeof(int));           //TODO : free done
    cscConfigMatrix.values = (int *)malloc(cscConfigMatrix.nz * sizeof(int));         //TODO : free done
   

    configurationMatrix(nclusters, &cscConfigMatrix);
    // printCscMatrix(&cscConfigMatrix);

    gettimeofday(&end, NULL);
    printf("Time for configuration matrix creation: %.5f seconds.\n", measureTime(begin, end));

    printf("Transposing the configuration matrix ...");
    gettimeofday(&begin, NULL);


    // Create the transpose of the configuration matrix
    CSR csrConfigTMatrix;

    transposeConfigurationMatrix(&cscConfigMatrix, &csrConfigTMatrix);
    // printCsrMatrix(&csrConfigTMatrix);

    gettimeofday(&end, NULL);
    printf("Time for configuration matrix transpose: %.5f seconds.\n", measureTime(begin, end));


    printf("Multiplying Omega T and initial matrix ...\n");
    gettimeofday(&begin, NULL);

    CSR configTProduct;

    // Multiply the transpose of the configuration matrix with the original matrix
    csrCscMultiplication(&csrConfigTMatrix, &cscMatrix, &configTProduct);

    // printCsrMatrix(&configTProduct);
    // printDenseCSRMatrix(&configTProduct);

    CSR csrFinalProduct;

    // Multiply the configuration matrix with the result of the previous multiplication
    printf("Multiplying Omega and the result of the previous multiplication ...\n");

    csrCscMultiplication(&configTProduct, &cscConfigMatrix, &csrFinalProduct); 
    // printCsrMatrix(&csrFinalProduct);
    // printDenseCSRMatrix(&csrFinalProduct);

    gettimeofday(&end, NULL);
    gettimeofday(&totalEnd, NULL);
    printf("Time for matrix multiplication: %.5f seconds.\n", measureTime(begin, end));
    printf("\nTotal time: %.5f seconds.\n", measureTime(totalBegin, totalEnd));

    fclose(fptr); 

    free(cscMatrix.cols);
    free(cscMatrix.rows);
    free(cscConfigMatrix.cols);
    free(cscConfigMatrix.rows);
    free(csrConfigTMatrix.cols); 
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "../read_matrix.h"
#include "../configuration_matrix.h"
#include "csr_csc_multiplication_cilk.h"
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
    if (argc != 4) {
        printf("\n\n    Usage: ./sequential <path_to_graph> <path_to_cluster_file> <number_of_tasks>\n\n");
        return 1;
    }

    FILE *fptr;

    printf("\n\n    OPEN CILK\n\n");
    printf("    Reading matrix %s ...\n", argv[1]);
    fptr = fopen (argv[1] , "r");
    
    if (fptr == NULL){
        printf("\n\n    Error! opening matrix file\n\n");
        // Program exits if file pointer returns NULL.
        return 1;
    }
    
    int requestedNumTasks = atoi(argv[3]);

    struct timeval begin, end, totalBegin, totalEnd;

    gettimeofday(&begin, NULL);
    gettimeofday(&totalBegin, NULL);

    int nrows, ncols , nz;
    

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
    printf("    Time for matrix read: %.5f seconds.\n\n", measureTime(begin, end));


    printf("    Creating configuration matrix...\n");
    gettimeofday(&begin, NULL);

    // Create the configuration matrix
    CSC cscConfigMatrix;
    
    FILE *fptrConfig;

    fptrConfig = fopen (argv[2] , "r");

    if (fptrConfig == NULL){
        printf("\n\n    Error! opening cluster file\n\n");
        // Program exits if file pointer returns NULL.
        return 1;
    }
   

    configurationMatrix(fptrConfig, &cscConfigMatrix, cscMatrix.ncols);
    // printCscMatrix(&cscConfigMatrix);

    gettimeofday(&end, NULL);
    printf("    Time for configuration matrix creation: %.5f seconds.\n\n", measureTime(begin, end));

    printf("    Transposing the configuration matrix ...\n");
    gettimeofday(&begin, NULL);


    // Create the transpose of the configuration matrix
    CSR csrConfigTMatrix;

    transposeConfigurationMatrix(&cscConfigMatrix, &csrConfigTMatrix);
    // printCsrMatrix(&csrConfigTMatrix);

    gettimeofday(&end, NULL);
    printf("    Time for configuration matrix transpose: %.5f seconds.\n\n", measureTime(begin, end));


    printf("    Multiplying Omega T and initial matrix ...\n");
    gettimeofday(&begin, NULL);

    CSR configTProduct;

    // Multiply the transpose of the configuration matrix with the original matrix
    csrCscMultiplication(&csrConfigTMatrix, &cscMatrix, &configTProduct, requestedNumTasks);

    // printCsrMatrix(&configTProduct);
    // printDenseCSRMatrix(&configTProduct);

    CSR csrFinalProduct;

    // Multiply the configuration matrix with the result of the previous multiplication
    printf("    Multiplying Omega and the result of the previous multiplication ...\n\n");

    csrCscMultiplication(&configTProduct, &cscConfigMatrix, &csrFinalProduct, requestedNumTasks); 

    gettimeofday(&end, NULL);
    gettimeofday(&totalEnd, NULL);
    printf("    Time for matrix multiplication: %.5f seconds.\n\n", measureTime(begin, end));
    printf("    Total time: %.5f seconds.\n", measureTime(totalBegin, totalEnd));

    // printCsrMatrix(&csrFinalProduct);
    // printDenseCSRMatrix(&csrFinalProduct);

    fclose(fptr);
    fclose(fptrConfig);

    free(cscMatrix.cols);
    free(cscMatrix.rows);
    free(cscConfigMatrix.cols);
    free(cscConfigMatrix.rows);
    free(csrConfigTMatrix.cols); 
}
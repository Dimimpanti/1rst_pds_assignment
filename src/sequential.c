#include <stdio.h>
#include <stdio.h>

#include "read_matrix.h"
#include "configuration_matrix.h"
#include "structs.h"


int main(){

FILE *fptr;
    fptr = fopen ("./../graphs/com_youtube/com_youtube.mtx" , "r");
    
    if (fptr == NULL){
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        return 1;
    }

    int nrows, ncols , nz , nclusters;    
    readHeader(&nrows, &ncols, &nz, fptr);

    CSR csrMatrix , csrMatrix1;
    
    csrMatrix.cols = (int *)malloc(2 * nz * sizeof(int));
    csrMatrix.rows = (int *)malloc((nrows + 1) * sizeof(int));
    csrMatrix.nrows= nrows;
    csrMatrix.nz = nz;

    createCsrMatrix(&csrMatrix, fptr, nz, nrows);
    printCsrMatrix(csrMatrix, nrows);

    configurationMatrix(nclusters, nrows , &csrMatrix1);
    
    fclose(fptr); 

    free(csrMatrix.cols);
    free(csrMatrix.rows);
}
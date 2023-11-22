#include <stdio.h>
#include <stdlib.h>
#include "graph_minor.h"


int isInRow(int *row, int rowSize, int col){
    for (int i = 0; i < rowSize; i++){
        if (row[i] == col){
            return 1;
        }
    }
    return 0;
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void computeGraphMinor(CSR *matrix, CSR *configurationMatrix, CSR *graphMinor){

        // print information about the matrices
    printf("\n");
    printf("        Input Matrix:\n");
    printf("            Number of rows: %d\n", matrix->nrows);
    printf("            Number of columns: %d\n", matrix->ncols);
    printf("            Number of non-zero elements: %d\n", matrix->nz);
    printf("        Configuration Matrix:\n");
    printf("            Number of rows: %d\n", configurationMatrix->nrows);
    printf("            Number of columns: %d\n", configurationMatrix->ncols);
    printf("            Number of non-zero elements: %d\n", configurationMatrix->nz);
    printf("        Processing...\n");
    
    
    int *ii = (int *)malloc(matrix->nz * sizeof(int));  // TODO : free done
    int *jj = (int *)malloc(matrix->nz * sizeof(int));  // TODO : free done
    int *vv = (int *)malloc(matrix->nz * sizeof(int));  // TODO : free done

    int col;

    // fill the ii(cols) and jj(rows) arrays from the configuration matrix
    for (int i = 0; i < matrix->nz; i++) {
        col = matrix->cols[i];

        ii[i] = configurationMatrix->cols[col];
        vv[i] = 1;
    }

    int rowIndex = 0;

    int *nnzInRow = (int *)calloc(matrix->nrows, sizeof(int));  // TODO : free done

    int nz_temp;

    for (int row = 0; row < matrix->nrows; row++) {
        nz_temp = matrix->rows[row + 1] - matrix->rows[row];

        for (int j = 0; j < nz_temp; j++) {
            jj[rowIndex] = configurationMatrix->cols[row];
            
            nnzInRow[jj[rowIndex]]++;

            rowIndex++;
        }
    }

    // The rows and columns of the graph minor are equal to the number of clusters
    graphMinor->nrows = configurationMatrix->ncols;
    graphMinor->ncols = configurationMatrix->ncols;

    int **minor = (int **)malloc(graphMinor->nrows * sizeof(int *));  // TODO : free done

    for (int i = 0; i < graphMinor->nrows; i++) {
        minor[i] = (int *)calloc(nnzInRow[i], sizeof(int));  // TODO : free done
    }

    int *minorRows = (int *)calloc(graphMinor->nrows, sizeof(int));  // TODO : free done

    int colToBeInserted;
    int rowToBeInserted;
    int index;
    graphMinor->nz = 0;

    // put ii(cols) and jj(rows) in the minor matrix without duplicates
    for (int nz = 0; nz < matrix->nz; nz++) {
        colToBeInserted = ii[nz];
        rowToBeInserted = jj[nz];
        index = minorRows[jj[nz]];

        if (isInRow(minor[rowToBeInserted], index, colToBeInserted)) {
            // add one to the value of the element
            vv[nz]++;
            continue;
        } 
        else {
            minor[rowToBeInserted][index] = ii[nz];
            minorRows[jj[nz]]++;
            graphMinor->nz++;
        }
    }

    // sort the rows of the minor matrix
    for (int i = 0; i < graphMinor->nrows; i++) {
        qsort(minor[i], minorRows[i], sizeof(int), cmpfunc);
    }


    // allocate memory for the arrays of the graph minor matrix
    graphMinor->rows = (int *)malloc((graphMinor->nrows + 1) * sizeof(int));  // TODO : free done
    graphMinor->values = (int *)malloc(graphMinor->nz * sizeof(int));         // TODO : free done
    graphMinor->cols = (int *)malloc(matrix->nz * sizeof(int));               // TODO : free done

    int resIndex = 0;

    // save the minor matrix in the graphMinor matrix
    for (int row = 0; row < graphMinor->nrows; row++) {
        graphMinor->rows[row] = resIndex;

        for (int col = 0; col < minorRows[row]; col++) {
            graphMinor->cols[resIndex] = minor[row][col];
            graphMinor->values[resIndex] = vv[resIndex];
            resIndex++;
        }
    }

    graphMinor->rows[graphMinor->nrows] = resIndex;

    printf("        Graph Minor Matrix:\n");
    printf("            Number of rows: %d\n", graphMinor->nrows);
    printf("            Number of columns: %d\n", graphMinor->ncols);
    printf("            Number of non-zero elements: %d\n", graphMinor->nz);
    printf("\n\n");

    // free the memory of the arrays
    free(ii);
    free(jj);
    free(vv);

    for (int i = 0; i < graphMinor->nrows; i++) {
        free(minor[i]);
    }

    free(minor);
    free(minorRows);
    free(nnzInRow);

}


/**
 * @brief Prints the dense matrix from a csr matrix
 * 
 * @param csrMatrix  The CSR matrix
 */
void printDenseCSRMatrix(CSR *csrMatrix){

    int startCol = 0; // the starting index of the column of the CSR matrix
    int endCol = 0;  // the ending index of the column of the CSR matrix

    int rowIndex = 0; // the index of the row of the CSR matrix
    int colIndex = 0; // the index of the column of the CSR matrix

    int maxDigits = 0;

    // Find the maximum number of digits in the matrix
    for(int row = 0; row < csrMatrix->nrows; row++){

        startCol = csrMatrix->rows[rowIndex];
        endCol = csrMatrix->rows[rowIndex + 1] - 1;
        
        for (int col = 0; col < csrMatrix->ncols; col++){
            if (col == csrMatrix->cols[startCol + colIndex] && colIndex <= endCol){
                if (maxDigits < csrMatrix->values[startCol + colIndex]){
                    maxDigits = csrMatrix->values[startCol + colIndex];
                }
                colIndex++;
            }
        }
        rowIndex++;
        colIndex = 0;
    }

    // Find the number of digits of the maximum number
    int digits = 0;
    while(maxDigits != 0){
        maxDigits /= 10;
        digits++;
    }

    // Print the matrix
    rowIndex = 0;
    colIndex = 0;
    printf("\n");
    printf("        Dense CSR Matrix:\n");
    printf("            Number of rows: %d\n", csrMatrix->nrows);
    printf("            Number of columns: %d\n", csrMatrix->ncols);
    printf("            Number of non-zero elements: %d\n\n", csrMatrix->nz);

    for(int row = 0; row < csrMatrix->nrows; row++){

        startCol = csrMatrix->rows[rowIndex];
        endCol = csrMatrix->rows[rowIndex + 1] - 1;
        
        printf("            ");

        for (int col = 0; col < csrMatrix->ncols; col++){
            if (col == csrMatrix->cols[startCol + colIndex] && colIndex <= endCol){
                printf("%*d ", digits, csrMatrix->values[startCol + colIndex]);
                colIndex++;
            } else {
                printf("%*d ", digits, 0);
            }
        }
        printf("\n");

        rowIndex++;
        colIndex = 0;
    }

    printf("\n");
}

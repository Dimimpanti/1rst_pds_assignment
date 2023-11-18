#include <stdlib.h>
#include <stdio.h>
#include "csr_csc_multiplication.h"


/**
 * @brief Multiplies a row of a CSR matrix with a column of a CSC matrix
 * 
 * @param startRow  The starting index of the row of the CSR matrix
 * @param endRow    The ending index of the row of the CSR matrix
 * @param startCol  The starting index of the column of the CSC matrix
 * @param endCol    The ending index of the column of the CSC matrix
 * @param row       The row of the CSR matrix
 * @param col       The column of the CSC matrix
 * @return int      The number of common elements between the row and the column
 */
int multiplyRowCol(int startRow , int endRow , int startCol , int endCol , int *row , int *col){

    int sum = 0;
    int row_index = startRow;
    int col_index = startCol;

    // Compare the elements of the row and the column
    while(row_index < endRow && col_index < endCol){

        // If the elements are equal, increase the sum and the indices
        if(row[row_index] == col[col_index]){
            sum++;
            row_index++;
            col_index++;
        }
        // If the element of the row is smaller than the element of the column, increase the row index
        else if(row[row_index] < col[col_index]){
            row_index++;
        }
        // If the element of the row is greater than the element of the column, increase the column index
        else{
            col_index++;
        }
    }

    // Return the sum
    return sum;
}


/**
 * @brief Multiplies a CSR matrix with a CSC matrix and outputs the result in a CSR matrix
 * 
 * @param csrMatrix  The CSR matrix
 * @param cscMatrix  The CSC matrix
 * @param output     The output matrix
 */
void csrCscMultiplication(CSR *csrMatrix , CSC *cscMatrix , CSR *output){

    int row_index = 0;
    int res_index = 0;
    int nnzInRow = 0;

    // Allocate the output matrix. The nz elements are the maximum number of non-zero elements that can be in the output matrix
    if (csrMatrix->nz > cscMatrix->nz){
        output->nz = csrMatrix->nz;
    } else {
        output->nz = cscMatrix->nz;
    }

    output->nrows = csrMatrix->nrows;
    output->cols = (int *)malloc(output->nz * sizeof(int));           //TODO : free
    output->rows = (int *)malloc((output->nrows + 1) * sizeof(int));  //TODO : free
    output->values = (int *)malloc(output->nz * sizeof(int));         //TODO : free

    output->rows[0] = 0;
    output->nz = 0;

    // For every row of the CSR matrix
    for(int row = 0; row < csrMatrix->nrows; row++){

        // The starting index of the row of the CSR matrix
        int startRow = csrMatrix->rows[row_index];

        // The ending index of the row of the CSR matrix
        int endRow = csrMatrix->rows[row_index + 1] - 1;

        // For every column of the CSC matrix
        for(int col = 0; col < cscMatrix->ncols; col++){

            // The starting index of the column of the CSC matrix
            int startCol = cscMatrix->cols[col];

            // The ending index of the column of the CSC matrix
            int endCol = cscMatrix->cols[col + 1] - 1;

            // Multiply the row with the column
            int res = multiplyRowCol(startRow , endRow , startCol , endCol , csrMatrix->cols , cscMatrix->rows);

            if (res != 0){
                // Store the column index
                output->cols[res_index] = col;

                // Store the value of the multiplication
                output->values[res_index] = res;

                // Increase the number of non-zero elements in the row
                nnzInRow++;

                // Increase the index of the output matrix
                res_index++;
            }
        }

        // Store the number of non-zero elements in the row
        output->rows[row + 1] = output->rows[row] + nnzInRow;

        // Set the number of non-zero elements in the output matrix
        output->nz += nnzInRow;

        // Increase the row index
        row_index++;

        // Reset the number of non-zero elements in the row
        nnzInRow = 0;
    }

    
}


/**
 * @brief Prints the dense matrix from a csr matrix
 * 
 * @param csrMatrix  The CSR matrix
 */
void printDenseMatrix(CSR *csrMatrix){

    int row_index = 0;
    int col_index = 0;

    // For every row of the CSR matrix
    for(int row = 0; row < csrMatrix->nrows; row++){

        // The starting index of the row of the CSR matrix
        int startRow = csrMatrix->rows[row_index];

        // The ending index of the row of the CSR matrix
        int endRow = csrMatrix->rows[row_index + 1] - 1;

        // For every column of the CSR matrix
        for(int col = 0; col < csrMatrix->nrows; col++){

            // The starting index of the column of the CSR matrix
            int startCol = csrMatrix->cols[col_index];

            // The ending index of the column of the CSR matrix
            int endCol = csrMatrix->cols[col_index + 1] - 1;

            // If the row and the column have common elements, print the number of common elements
            if(row_index < csrMatrix->nrows && col_index < csrMatrix->nrows && row == csrMatrix->rows[row_index] && col == csrMatrix->cols[col_index]){
                printf("%d ", csrMatrix->values[col_index]);
                col_index++;
            }
            // If the row and the column do not have common elements, print 0
            else{
                printf("0 ");
            }
        }

        // Increase the row index
        row_index++;

        // Print a new line
        printf("\n");
    }
}


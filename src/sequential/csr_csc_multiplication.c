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
int multiplyRowCol(int startRow , int endRow , int startCol , int endCol , int *row , int *col, int *csrValues, int *cscValues){

    int sum = 0; // the sum of the multiplication of the common elements
    int row_index = startRow; //the index of the row we are looking at
    int col_index = startCol; //the index of the column we are looking at

    // Compare the elements of the row and the column
    while(row_index <= endRow && col_index <= endCol){

        // If the elements are equal, increase the sum and the indices
        if(row[row_index] == col[col_index]){
            sum += csrValues[row_index] * cscValues[col_index];
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


    // Allocate the output matrix. The nz elements are the maximum number of non-zero elements that can be in the output matrix
    if (csrMatrix->nz > cscMatrix->nz){
        output->nz = csrMatrix->nz;
    } else {
        output->nz = cscMatrix->nz;
    }

    output->nrows = csrMatrix->nrows;
    output->ncols = cscMatrix->ncols;
    output->cols = (int *)malloc(output->nz * sizeof(int));           //TODO : free
    output->rows = (int *)malloc((output->nrows + 1) * sizeof(int));  //TODO : free
    output->values = (int *)malloc(output->nz * sizeof(int));         //TODO : free

    output->rows[0] = 0; // the first element of the rows array at a Csr matrix is 0
    output->nz = 0;

    int startRow = 0; // the starting index of the row of the CSR matrix
    int endRow = 0; // the ending index of the row of the CSR matrix

    int startCol = 0; // the starting index of the column of the CSC matrix
    int endCol = 0; // the ending index of the column of the CSC matrix
 
    int res = 0;   // the result of the multiplication of the row with the column
    int res_index = 0; // the index of the output matrix
    int nnzInRow = 0; // the number of non-zero elements in the row

    // print information about the matrices
    printf("\n");
    printf("        CSR Matrix:\n");
    printf("            Number of rows: %d\n", csrMatrix->nrows);
    printf("            Number of columns: %d\n", csrMatrix->ncols);
    printf("            Number of non-zero elements: %d\n", csrMatrix->nz);
    printf("        CSC Matrix:\n");
    printf("            Number of rows: %d\n", cscMatrix->nrows);
    printf("            Number of columns: %d\n", cscMatrix->ncols);
    printf("            Number of non-zero elements: %d\n", cscMatrix->nz);
    printf("        Processing...\n");

    // For every row of the CSR matrix
    for(int row = 0; row < csrMatrix->nrows; row++){

        // The starting index of the row of the CSR matrix
        startRow = csrMatrix->rows[row];

        // The ending index of the row of the CSR matrix
        endRow = csrMatrix->rows[row + 1] - 1;

        // For every column of the CSC matrix
        for(int col = 0; col < cscMatrix->ncols; col++){

            // The starting index of the column of the CSC matrix
            startCol = cscMatrix->cols[col];

            // The ending index of the column of the CSC matrix
            endCol = cscMatrix->cols[col + 1] - 1;

            // Multiply the row with the column
            res = multiplyRowCol(startRow, endRow, startCol, endCol, csrMatrix->cols, cscMatrix->rows, csrMatrix->values, cscMatrix->values);

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

            // for every 200000 cols print the progress
            // if (col % 200000 == 0 || col == cscMatrix->ncols - 1 || col == 0){
            //     printf("            Row %d Progress: %.2f%%", row, (float)col / (cscMatrix->ncols - 1) * 100);
            //     printf("\r");
            //     fflush(stdout);
            // }
        }

        // Store the number of non-zero elements in the row
        output->rows[row + 1] = output->rows[row] + nnzInRow;

        // Set the number of non-zero elements in the output matrix
        output->nz += nnzInRow;

        // Reset the number of non-zero elements in the row
        nnzInRow = 0;

        // printf("\r                                                                  ");
        // printf("\r");
    }
    printf("        Output Matrix:\n");
    printf("            Number of rows: %d\n", output->nrows);
    printf("            Number of columns: %d\n", output->ncols);
    printf("            Number of non-zero elements: %d\n", output->nz);
    printf("\n\n");
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


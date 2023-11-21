#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "csr_csc_multiplication_pthread.h"

#define NUM_THREADS 128

typedef struct ThreadArgs{
    CSR *csrMatrix; 
    CSC *cscMatrix;
    int *outputCols;
    int *outputValues;
    int *outputRows;
    int nz; 
    int startRow; // the start row is included
    int endRow;   // the end row is included
    int threadId; 
} ThreadArgs;

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
    int row_index = startRow;  //the index of the row we are looking at
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
 * @brief Thread function for the multiplication of a row of a CSR matrix with a column of a CSC matrix
 *  
 * @param args  The arguments of the thread 
 */
void *csrCscMultiplicationRunnable(void *args){

    ThreadArgs *threadArgs = (ThreadArgs *)args;

    CSR *csrMatrix = threadArgs->csrMatrix;
    CSC *cscMatrix = threadArgs->cscMatrix;

    int startRow = 0; // the starting index of the row of the CSR matrix
    int endRow = 0; // the ending index of the row of the CSR matrix

    int startCol = 0; // the starting index of the column of the CSC matrix
    int endCol = 0; // the ending index of the column of the CSC matrix

    int res = 0; // the result of the multiplication of the row with the column
    int resIndex = 0; // the index of the output matrix
    int rowIndex = 0; //the row index of the output matrix
    int nnzInRow = 0; // the number of non-zero elements in the row of the output matrix

    threadArgs->outputRows[0] = 0;
    threadArgs->nz = 0;

    // For every row of the CSR matrix that the thread is responsible for 
    for(int row = threadArgs->startRow; row <= threadArgs->endRow; row++){

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
                threadArgs->outputCols[resIndex] = col;

                // Store the value of the multiplication
                threadArgs->outputValues[resIndex] = res;

                // if (threadArgs->threadId == 0)
                //     printf("Thread %d: row %d col %d res %d resIndex %d\n", threadArgs->threadId, row, col, res, resIndex);

                // Increase the number of non-zero elements in the row
                nnzInRow++;

                // Increase the index of the output matrix
                resIndex++;
            }
        }

        // Store the number of non-zero elements in the row
        threadArgs->outputRows[rowIndex + 1] = threadArgs->outputRows[rowIndex] + nnzInRow;
        
        rowIndex++; // Increment the row index of the output matrix in order to go to the next row
        
        // Set the number of non-zero elements in the output matrix
        threadArgs->nz += nnzInRow;

        // Reset the number of non-zero elements in the row
        nnzInRow = 0;
    }

    pthread_exit(NULL);
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

    output->nrows = csrMatrix->nrows; // the number of rows of the output matrix is the same as the number of rows of the CSR matrix
    output->ncols = cscMatrix->ncols; // the number of columns of the output matrix is the same as the number of columns of the CSC matrix
    output->cols = (int *)malloc(output->nz * sizeof(int));           //TODO : free
    output->rows = (int *)malloc((output->nrows + 1) * sizeof(int));  //TODO : free
    output->values = (int *)malloc(output->nz * sizeof(int));         //TODO : free
    output->nz = 0;
    

    // Initialize the thread attributes
    pthread_attr_t pthreadCustomAttr;
    pthread_attr_init(&pthreadCustomAttr);

    // decide the number of threads to create
    int numThreads = NUM_THREADS;

    if (numThreads > csrMatrix->nrows){
        numThreads = csrMatrix->nrows;
    }

    int rowsPerThread = csrMatrix->nrows / numThreads; // the number of rows that each thread will be responsible for

    int rowsRemaining = csrMatrix->nrows % numThreads; // the remaining rows that have to be assigned to the threads

    int *rowsPerThreadArray = (int *)malloc(numThreads * sizeof(int));  //TODO : free done
    
    for(int i = 0; i < numThreads; i++){
        rowsPerThreadArray[i] = rowsPerThread;
        
        if(rowsRemaining > 0){
            rowsPerThreadArray[i]++;
            rowsRemaining--;
        } 
    }

    // Create the threads
    pthread_t *threads = (pthread_t *)malloc(numThreads * sizeof(pthread_t));  //TODO : free done

    // Create the arguments for the threads
    ThreadArgs *threadArgs = (ThreadArgs *)malloc(numThreads * sizeof(ThreadArgs));  //TODO : free done

    int maxNonZeroElements = 0; //the dimension of the outputCols and outputValues arrays

    for (int threadId = 0; threadId < numThreads; threadId++){

        threadArgs[threadId].csrMatrix = csrMatrix;
        threadArgs[threadId].cscMatrix = cscMatrix;
        threadArgs[threadId].outputRows = (int *)malloc((rowsPerThreadArray[threadId] + 1) * sizeof(int));  //TODO : free done
        threadArgs[threadId].threadId = threadId;

        if(threadId == 0 ){
            threadArgs[threadId].startRow = 0; 
        }
        else {
            threadArgs[threadId].startRow = threadArgs[threadId - 1].endRow + 1;
        }
        
        threadArgs[threadId].endRow = threadArgs[threadId].startRow + rowsPerThreadArray[threadId] - 1;

        if(csrMatrix->rows[threadArgs[threadId].endRow + 1] - csrMatrix->rows[threadArgs[threadId].startRow] > cscMatrix->nz){
            maxNonZeroElements = csrMatrix->rows[threadArgs[threadId].endRow + 1] - csrMatrix->rows[threadArgs[threadId].startRow];
        }
        else{
            maxNonZeroElements = cscMatrix->nz;
        } 


        threadArgs[threadId].outputCols = (int *)malloc(maxNonZeroElements * sizeof(int));      // TODO : free done
        threadArgs[threadId].outputValues = (int *)malloc(maxNonZeroElements * sizeof(int));    // TODO : free done


        pthread_create(&threads[threadId], &pthreadCustomAttr, csrCscMultiplicationRunnable, &threadArgs[threadId]);
    }

    // Join the threads
    for (int threadId = 0; threadId < numThreads; threadId++){
        pthread_join(threads[threadId], NULL);
    }
    
    int lastElement = 0;
    int rowsIndex = 0; //the index of the output rows array
    int colIndex = 0; //the index of the output cols and values arrays

    // Merge the results of the threads
    for(int threadId = 0; threadId < numThreads; threadId++){
        
        //merge the output rows for all the threads
        if(threadId == 0){
            //if thread 0 we copy the array as it is 
            for (int i = 0; i <= rowsPerThreadArray[threadId]; i++){
                output->rows[rowsIndex] = threadArgs[threadId].outputRows[i];
                rowsIndex++;
            }
            lastElement = threadArgs[threadId].outputRows[rowsPerThreadArray[threadId]];
        }
        else{
            for(int i = 1; i <= rowsPerThreadArray[threadId]; i++){
                output->rows[rowsIndex] = threadArgs[threadId].outputRows[i] + lastElement;
                rowsIndex++;
            }
            lastElement += threadArgs[threadId].outputRows[rowsPerThreadArray[threadId]];
        }
    
        //merge the output columns for all the threads
        for(int i = 0; i < threadArgs[threadId].nz ; i++){
            output->cols[colIndex] = threadArgs[threadId].outputCols[i];
            output->values[colIndex] = threadArgs[threadId].outputValues[i];
            colIndex++;
        }

        output->nz += threadArgs[threadId].nz;
    }

    free(threads);
    free(rowsPerThreadArray);
    
    // Free the thread attributes
    pthread_attr_destroy(&pthreadCustomAttr);

    // Free the thread arguments
    for (int threadId = 0; threadId < numThreads; threadId++){
        free(threadArgs[threadId].outputCols);
        free(threadArgs[threadId].outputValues);
        free(threadArgs[threadId].outputRows);
    }

    // Free the thread arguments
    free(threadArgs);
}


/**
 * @brief Prints the dense matrix from a csr matrix
 * 
 * @param csrMatrix  The CSR matrix
 */
void printDenseCSRMatrix(CSR *csrMatrix){

    int startCol = 0; // the starting index of the column of the CSR matrix
    int endCol = 0; // the ending index of the column of the CSR matrix

    int rowIndex = 0; // the index of the row we are looking at at the CSR matrix
    int colIndex = 0; // the index of the column we are looking at at the CSR matrix

    // For every row of the CSR matrix
    for(int row = 0; row < csrMatrix->nrows; row++){

        startCol = csrMatrix->rows[rowIndex];
        endCol = csrMatrix->rows[rowIndex + 1] - 1;
        
        for (int col = 0; col < csrMatrix->ncols; col++){
            if (col == csrMatrix->cols[startCol + colIndex] && colIndex <= endCol){
                printf("%d ", csrMatrix->values[startCol + colIndex]);
                colIndex++;
            } else {
                printf("0 ");
            }
        }
        printf("\n");

        rowIndex++;
        colIndex = 0;
    }
}


#ifndef STRUCTS_H   /* Include guard */
#define STRUCTS_H 

/**
 * @brief Struct for the CSR matrix
 * 
 */
typedef struct CSR{
    int *rows;    // The array that stores the number of non-zero elements since the ith row
    int *cols;    // The array that stores the column index of the non-zero elements
    int *values;  // The array that stores the values of the non-zero elements
    int nz;       // The number of non-zero elements in the matrix
    int nrows;    // The number of rows in the matrix
    int ncols;    // The number of columns in the matrix
} CSR;

/**
 * @brief Struct for the CSC matrix
 * 
 */
typedef struct CSC{
    int *rows;    // The array that stores the row index of the non-zero elements
    int *cols;    // The array that stores the number of non-zero elements since the ith column
    int *values;  // The array that stores the values of the non-zero elements
    int nz;       // The number of non-zero elements in the matrix
    int nrows;    // The number of rows in the matrix
    int ncols;    // The number of columns in the matrix
} CSC;

#endif
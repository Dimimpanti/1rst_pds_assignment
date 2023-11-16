#ifndef READ_MATRIX_H   /* Include guard */
#define READ_MATRIX_H 

#include <stdio.h>

typedef struct CSR{
    int *rows;
    int *cols;
    int nz;
} CSR;

void readHeader(int *nrows, int *ncols ,int *nz , FILE *initialFile);

void createCsrMatrix(CSR *csrMatrix, FILE  *initialFile, int nz, int nrows);

#endif
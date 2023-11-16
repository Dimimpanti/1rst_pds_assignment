#ifndef READ_MATRIX_H   /* Include guard */
#define READ_MATRIX_H 

#include <stdio.h>

void readHeader(int *nrows, int *ncols ,int *nz , FILE *initialFile);

void createCsrMatrix(int *csrRows, int *csrCols, FILE  *initialFile, int nz, int nrows);

#endif
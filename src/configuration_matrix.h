#ifndef CONFIGURATION_MATRIX_H   /* Include guard */
#define CONFIGURATION_MATRIX_H 

#include <stdio.h>


void printMatrixΩ(int *matrix, int rows, int cols);
void CsrMatrixΩ(int *MatrΩ, int nrows, int clusters);
void MatrixΩ(int clusters, int nrows, FILE *initialFile);

#endif
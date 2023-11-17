#ifndef READ_MATRIX_H   /* Include guard */
#define READ_MATRIX_H 

#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "configuration_matrix.h"

void readHeader(int *nrows, int *ncols ,int *nz , FILE *initialFile);

void skipHeader(FILE *initialFile);

void createCsrMatrix(CSR *csrMatrix, FILE  *initialFile, int nz, int nrows);

void countElemPerRow(FILE *initialFile, int *elemPerRowDown, int *elemPerRowUp, int nz);

ElementsOfGraph **allocateGraph(int nrows, int *elemPerRow);

void createGraph(FILE *initialFile, ElementsOfGraph **LowerGraph, ElementsOfGraph **UpperGraph, int nz, int nrows);

void createCsrMatrix(CSR *csrMatrix, FILE  *initialFile, int nz, int nrows);

void printCsrMatrix(CSR csrMatrix, int nrows);

#endif 
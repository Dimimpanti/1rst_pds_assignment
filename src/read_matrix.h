#ifndef READ_MATRIX_H   /* Include guard */
#define READ_MATRIX_H 

#include <stdio.h>

#include "structs.h"


/**
 * @brief struct that includes the pair of row and col we read from the file
 */
typedef struct ElementsOfGraph {
    int col;  //column of the vertex of the graph
    int row; //row of the vertex of the graph
} ElementsOfGraph;


void readHeader(int *nrows, int *ncols ,int *nz , FILE *initialFile);

// void skipHeader(FILE *initialFile);

void createCsrMatrix(CSR *csrMatrix, FILE  *initialFile, int nz, int nrows);

void createCscMatrix(CSC *cscMatrix, FILE  *initialFile, int nz, int nrows);

// void countElemPerRow(FILE *initialFile, int *elemPerRowDown, int *elemPerRowUp, int nz);

// ElementsOfGraph **allocateGraph(int nrows, int *elemPerRow);

// void createGraph(FILE *initialFile, ElementsOfGraph **LowerGraph, ElementsOfGraph **UpperGraph, int nz, int nrows);

void printCsrMatrix(CSR *csrMatrix);

void printCscMatrix(CSC *cscMatrix);

#endif 
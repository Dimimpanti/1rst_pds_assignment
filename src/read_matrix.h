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

void createCsrMatrix(CSR *csrMatrix, FILE  *initialFile);

void createCscMatrix(CSC *cscMatrix, FILE  *initialFile);

void printCsrMatrix(CSR *csrMatrix);

void printCscMatrix(CSC *cscMatrix);

#endif 
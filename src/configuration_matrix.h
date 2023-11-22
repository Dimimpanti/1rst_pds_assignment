#ifndef CONFIGURATION_MATRIX_H   /* Include guard */
#define CONFIGURATION_MATRIX_H 

#include <stdio.h>

#include "structs.h"

void csrConfigurationMatrix(FILE *clusteringFile, CSR *csrMatrix, int nNodes);

void configurationMatrix(FILE *clusteringFile, CSC *cscMatrix, int nNodes);

void transposeConfigurationMatrix(CSC *input , CSR *output);
#endif
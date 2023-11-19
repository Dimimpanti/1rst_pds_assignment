#ifndef CONFIGURATION_MATRIX_OPENMP_H   /* Include guard */
#define CONFIGURATION_MATRIX_OPENMP_H 

#include <stdio.h>

#include "../structs.h"


void configurationMatrix(int nclusters, CSC *cscMatxix);

void transposeConfigurationMatrix(CSC *input , CSR *output);
#endif
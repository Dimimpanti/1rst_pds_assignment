#ifndef CSR_CSC_MULTIPLICATION_OPENMP_H   /* Include guard */
#define CSR_CSC_MULTIPLICATION_OPENMP_H

#include "../structs.h"

void csrCscMultiplication(CSR *csrMatrix , CSC *cscMatrix , CSR *output, int numThreads);

void printDenseCSRMatrix(CSR *csrMatrix);

#endif




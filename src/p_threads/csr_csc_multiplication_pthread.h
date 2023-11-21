#ifndef CSR_CSC_MULTIPLICATION_PTHREAD_H   /* Include guard */
#define CSR_CSC_MULTIPLICATION_PTHREAD_H

#include "../structs.h"

void csrCscMultiplication(CSR *csrMatrix , CSC *cscMatrix , CSR *output, int requestedThreads);

void printDenseCSRMatrix(CSR *csrMatrix);

#endif




#ifndef CSR_CSC_MULTIPLICATION_CILK_H   /* Include guard */
#define CSR_CSC_MULTIPLICATION_CILK_H

#include "../structs.h"

void csrCscMultiplication(CSR *csrMatrix , CSC *cscMatrix , CSR *output);

void printDenseCSRMatrix(CSR *csrMatrix);

#endif




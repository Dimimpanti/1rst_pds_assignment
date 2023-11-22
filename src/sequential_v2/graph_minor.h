#ifndef GRAPH_MINOR_H
#define GRAPH_MINOR_H

#include "../structs.h"


void computeGraphMinor(CSR *matrix, CSR *configurationMatrix, CSR *graphMinor);

void printDenseCSRMatrix(CSR *csrMatrix);

#endif
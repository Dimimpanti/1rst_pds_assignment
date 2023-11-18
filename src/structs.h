#ifndef STRUCTS_H   /* Include guard */
#define STRUCTS_H 

typedef struct CSR{
    int *rows;
    int *cols;
    int *values;
    int nz;
    int nrows;
} CSR;

typedef struct CSC{
    int *rows;
    int *cols;
    int *values;
    int nz;
    int ncols;
} CSC;

#endif
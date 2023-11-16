#ifndef STRUCTS_H   /* Include guard */
#define STRUCTS_H 


typedef struct CSR{
    int *rows;
    int *cols;
    int nz;
    int nrows;
} CSR;

#endif
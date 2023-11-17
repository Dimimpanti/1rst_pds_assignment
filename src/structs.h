#ifndef STRUCTS_H   /* Include guard */
#define STRUCTS_H 


typedef struct CSR{
    int *rows;
    int *cols;
    int nz;
    int nrows;
} CSR;

typedef struct ElementsOfGraph{
    int col;  //column of the vertex of the graph
    int row; //row of the vertex of the graph
} ElementsOfGraph;


#endif
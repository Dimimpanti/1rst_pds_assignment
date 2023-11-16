#include <stdio.h>
#include "read_matrix.h"

//struct that includes the pair of row and col we read from the file
struct ElementsOfGraph {
    int col;
    int row;
};


//Matrix with dimensions  nrows x elemPerRow[i]->elements from the elemPerRowDown matrix 
int **Graph(int nrows, struct ElementsOfGraph *elements, int *elemPerRow) {
    
   
    int **graph = (int **)malloc(sizeof(int *) * nrows);

     //Allocates memory for each row of the graph
    for (int i = 0; i < nrows; i++) {
        graph[i] = (int *)malloc(sizeof(int) * elemPerRow[i]);
    }
    return graph;
}

void printGraph(int nrows, int **graph, int *elemPerRow) {
    

    for (int i = 0; i < nrows; i++) {
        printf("Row %d: ", i + 1);
        for (int j = 0; j < elemPerRow[i]; j++) {
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }
}


//stores the pair of row and col we read from the file to the struct we built
void saveRowsAndCols(FILE *initialFile, int *elemPerRow, int nz){
    
    rewind(initialFile);
        
    skipHeader(initialFile);

    int row, col ;
    struct ElementsOfGraph s;

    for(int i = 0; i < nz; i++){
        
        fscanf(initialFile, "%d %d ", &row, &col);

        s.row=row;
        s.col=col;
        /////////////////////////
    ///////////////////////////
    }


}




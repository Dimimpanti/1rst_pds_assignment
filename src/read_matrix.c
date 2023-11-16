#include "read_matrix.h"
#include <stdlib.h>


//struct that includes the pair of row and col we read from the file
typedef struct ElementsOfGraph {
    int col;
    int row;
} ElementsOfGraph;



//skip the comments of the mtx file and get the nrows , ncols ,nz
void readHeader(int *nrows, int *ncols ,int *nz , FILE *initialFile){
    char cursor;
    do{
        fscanf(initialFile,"%c", &cursor);
        if(cursor == '%'){
            do{
                fscanf(initialFile,"%c", &cursor);
            }while(cursor != '\n');

        }
        else {
            break;
        }
    }while(1);

    fseek(initialFile , -1 , SEEK_CUR);

    fscanf(initialFile, "%d %d %d", nrows, ncols, nz);
    
    // printf("%d %d %d\n", *nrows, *ncols, *nz);

}

void skipHeader(FILE *initialFile){
    char cursor;
    do{
        fscanf(initialFile,"%c", &cursor);
        if(cursor == '%'){
            do{
                fscanf(initialFile,"%c", &cursor);
            }while(cursor != '\n');

        }
        else {
            break;
        }
    }while(1);

    fseek(initialFile , -1 , SEEK_CUR);

    int t1 ,t2 ,t3;
    fscanf(initialFile, "%d %d %d", &t1, &t2, &t3);
}


/**
 * @brief counting the non-zero elements of the array A
 * 
 * @param initialFile 
 * @param elemPerRowDown 
 * @param elemPerRowUp 
 * @param nz 
 */
void countElemPerRow(FILE *initialFile, int *elemPerRowDown, int *elemPerRowUp, int nz){
    
    // rewind(initialFile);
    fseek(initialFile, 0, SEEK_SET);
        
    skipHeader(initialFile);

    int row, col;


    //reading the pairs of rows and cols and rows++ and cols++
    for(int i = 0; i < nz; i++){
        
        fscanf(initialFile, "%d %d ", &row, &col);

        // printf("Row: %d, Col: %d\n", row, col);
        
        //increasing the pair of row and col I read from the file in order to find the U,D matrices
        elemPerRowDown[row - 1]++;//row-1 -> 0indexed D
        elemPerRowUp[col - 1]++;//U
    }
}

/**
 * @brief Allocates memory for the half of graph with dimensions  
 * nrows x elemPerRow[i]->elements from the elemPerRowDown matrix 
 * 
 * @param nrows 
 * @param elements 
 * @param elemPerRow 
 * @return 
 */
ElementsOfGraph **allocateGraph(int nrows, int *elemPerRow) {
    
   
    ElementsOfGraph **graph = (ElementsOfGraph **)malloc(sizeof(ElementsOfGraph *) * nrows);

    //Allocates memory for each row of the graph
    for (int i = 0; i < nrows; i++) {
        graph[i] = (ElementsOfGraph *)malloc(sizeof(ElementsOfGraph) * elemPerRow[i]);
    }

    // return the pointer
    return graph;
}

//stores the pair of row and col we read from the file to the struct we built
void createGraph(FILE *initialFile, ElementsOfGraph **LowerGraph, ElementsOfGraph **UpperGraph, int nz, int nrows){
    
    // rewind(initialFile);
    fseek(initialFile, 0, SEEK_SET);

        
    skipHeader(initialFile);

    int read_row, read_col;

    int *lowerColumnIndex = (int *)calloc(nrows, sizeof(int));
    int *upperColumnIndex = (int *)calloc(nrows, sizeof(int));

    for(int i = 0 ;i < nz; i++){
        fscanf(initialFile, "%d %d ", &read_row, &read_col);

        //convert to 0indexed
        read_row--;
        read_col--;


        // first time run read_row = 1 and read_col = 0

        LowerGraph[read_row][lowerColumnIndex[read_row]].row = read_row;
        LowerGraph[read_row][lowerColumnIndex[read_row]].col = read_col;
        lowerColumnIndex[read_row]++;

        UpperGraph[read_col][upperColumnIndex[read_col]].row = read_col;
        UpperGraph[read_col][upperColumnIndex[read_col]].col = read_row;
        upperColumnIndex[read_col]++;

    }
    free(lowerColumnIndex);
    free(upperColumnIndex);
}



/**
 * @brief Create a Csr Matrix matrix from a file
 * 
 * @param rows rows vector of csr matrix
 * @param cols cols vector of csr matrix
 * @param fptr file pointer
 * @param nz number of nz elements
 */
 void createCsrMatrix(CSR *csrMatrix, FILE  *initialFile, int nz, int nrows){      
   
    int *elemPerRowDown = (int *)calloc(nrows, sizeof(int)); 
    int *elemPerRowUp = (int *)calloc(nrows, sizeof(int));
    
    
    countElemPerRow(initialFile, elemPerRowDown, elemPerRowUp, nz);  //Creating the U,D matrices
    
    ElementsOfGraph **LowerGraph = allocateGraph(nrows , elemPerRowDown);
    ElementsOfGraph **UpperGraph = allocateGraph(nrows , elemPerRowUp);

    createGraph(initialFile, LowerGraph, UpperGraph, nz, nrows);

    //int row_index = 0;
    int col_index = 0;
    csrMatrix->rows[0] = 0;

    for(int row = 0 ; row < nrows; row++){
        for(int col = 0; col < elemPerRowDown[row]; col++){
            csrMatrix->cols[col_index] = LowerGraph[row][col].col;
            col_index++;
        }

        for (int col = 0; col < elemPerRowUp[row]; col++) {
            csrMatrix->cols[col_index] = UpperGraph[row][col].col;
            col_index++;
        }

        csrMatrix->rows[row + 1] = elemPerRowDown[row] + elemPerRowUp[row];
        csrMatrix->rows[row + 1] += csrMatrix->rows[row];
    }
    free(elemPerRowDown);
    free(elemPerRowUp);

    
    for(int i = 0; i < nrows; i++){
        free(LowerGraph[i]);
        free(UpperGraph[i]);
    }
    free(LowerGraph);
    free(UpperGraph);
}

void printCsrMatrix(CSR csrMatrix, int nrows){
    for(int i = 0; i < nrows; i++){
        for(int j = csrMatrix.rows[i]; j < csrMatrix.rows[i + 1]; j++){
            printf("%d ", csrMatrix.cols[j]);
        }
    }
    printf("\n");

    for (int i = 0; i < nrows + 1; i++){
        printf("%d ", csrMatrix.rows[i]);
    }
    printf("\n");
    
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


int main(){
    
    FILE *fptr;
    fptr = fopen ("./../graphs/com_youtube/com_youtube.mtx" , "r");
    
    if (fptr == NULL){
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        return 1;
    }

    int nrows, ncols , nz;    
    readHeader(&nrows, &ncols, &nz, fptr);

    CSR csrMatrix;
    
    csrMatrix.cols = (int *)malloc(2 * nz * sizeof(int));
    csrMatrix.rows = (int *)malloc((nrows + 1) * sizeof(int));

    createCsrMatrix(&csrMatrix, fptr, nz, nrows);
    printCsrMatrix(csrMatrix, nrows);
   
    fclose(fptr); 

    free(csrMatrix.cols);
    free(csrMatrix.rows);
    

    return 0;
}
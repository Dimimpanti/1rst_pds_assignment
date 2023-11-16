#include "read_matrix.h"
#include <stdlib.h>


/**
 * @brief struct that includes the pair of row and col we read from the file
 */
typedef struct ElementsOfGraph {
    int col;  //column of the vertex of the graph
    int row; //row of the vertex of the graph
} ElementsOfGraph;



/**
 * @brief skip the comments of the mtx file and get the nrows , ncols ,nz
 * 
 * @param nrows  number of rows of the mtx file
 * @param ncols  number of cols of the mtx file
 * @param nz   number of non-zero elements of the mtx file
 * @param initialFile  file pointer of the mtx file we want to read
 */
void readHeader(int *nrows, int *ncols ,int *nz , FILE *initialFile){
    char cursor; //temporary variable to read the file

    //skip the comments of the mtx file. The comments start with %
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

    //return the pointer to the previous character
    fseek(initialFile , -1 , SEEK_CUR);

    //read the first line after the comments of the mtx file
    fscanf(initialFile, "%d %d %d", nrows, ncols, nz);
    
    // printf("%d %d %d\n", *nrows, *ncols, *nz);

}


/**
 * @brief skip the comments of the mtx file
 * 
 * @param initialFile  file pointer of the mtx file we want to read
 */
void skipHeader(FILE *initialFile){
    char cursor; //temporary variable to read the file

    //skip the comments of the mtx file. The comments start with %
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

    //return the pointer to the previous character
    fseek(initialFile , -1 , SEEK_CUR);

    //read the first line after the comments of the mtx file and do nothing with it
    int t1 ,t2 ,t3;
    fscanf(initialFile, "%d %d %d", &t1, &t2, &t3);
}


/**
 * @brief counting the non-zero elements of the Lower and Upper matrices per row
 * 
 * @param initialFile file pointer of the mtx file we want to read
 * @param elemPerRowDown array that stores the non-zero elements of the Lower matrix per row
 * @param elemPerRowUp array that stores the non-zero elements of the Upper matrix per row
 * @param nz  the non-zero elements of the mtx file
 */
void countElemPerRow(FILE *initialFile, int *elemPerRowDown, int *elemPerRowUp, int nz){
    
    //going back to the start of the file
    fseek(initialFile, 0, SEEK_SET);
        
    //skip the comments of the mtx file.
    skipHeader(initialFile);


    int row, col;//temporary variables to read the file


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
    csrMatrix.nrows= nrows;
    csrMatrix.nz = nz;

    createCsrMatrix(&csrMatrix, fptr, nz, nrows);
    printCsrMatrix(csrMatrix, nrows);
   
    fclose(fptr); 

    free(csrMatrix.cols);
    free(csrMatrix.rows);
    

    return 0;
}
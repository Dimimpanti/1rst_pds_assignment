#include <stdlib.h>

#include "read_matrix.h"
#include "configuration_matrix.h"


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
 * @brief  counting the non-zero elements of the Lower and Upper matrices per column
 * 
 * @param initialFile  file pointer of the mtx file we want to read
 * @param elemPerColDown  array that stores the non-zero elements of the Lower matrix per column
 * @param elemPerColUp  array that stores the non-zero elements of the Upper matrix per column
 * @param nz  the non-zero elements of the mtx file
 */
void countElemPerCol(FILE *initialFile, int *elemPerColDown, int *elemPerColUp, int nz){
    
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
        elemPerColDown[col - 1]++;//row-1 -> 0indexed D
        elemPerColUp[row - 1]++;//U
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

/**
 * @brief Create the final Graph (lower and upper combined)
 * 
 * @param initialFile file pointer of the mtx file we want to read
 * @param LowerGraph array that has the non-zero elements of the lower triangular matrix (the one we get from the file mtx)
 * @param UpperGraph array array that has the non-zero elements of the lower triangular matrix(symmetrical to the one we get from the file mtx)
 * @param nz number of non-zero elements of the mtx fil 
 * @param nrows number of rows of the mtx file
 * @return * stores struct 
 */
void createGraph(FILE *initialFile, ElementsOfGraph **LowerGraph, ElementsOfGraph **UpperGraph, int nz, int nrows){
    
    //going back to the start of the file
    fseek(initialFile, 0, SEEK_SET);

        
    skipHeader(initialFile);

    int read_row, read_col;

    //arrays of the columns of the two graphs 
    int *lowerColumnIndex = (int *)calloc(nrows, sizeof(int));
    int *upperColumnIndex = (int *)calloc(nrows, sizeof(int));

    for(int i = 0 ;i < nz; i++){
        fscanf(initialFile, "%d %d ", &read_row, &read_col);

        //convert to 0indexed
        read_row--;
        read_col--;

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


void creatGraphCol(FILE *initialFile, ElementsOfGraph **LowerGraph, ElementsOfGraph **UpperGraph, int nz, int nrows){
    
    //going back to the start of the file
    fseek(initialFile, 0, SEEK_SET);

        
    skipHeader(initialFile);

    int read_row, read_col;

    //arrays of the columns of the two graphs 
    int *lowerColumnIndex = (int *)calloc(nrows, sizeof(int));
    int *upperColumnIndex = (int *)calloc(nrows, sizeof(int));

    for(int i = 0 ;i < nz; i++){
        fscanf(initialFile, "%d %d ", &read_row, &read_col);

        //convert to 0indexed
        read_row--;
        read_col--; 

        LowerGraph[read_col][lowerColumnIndex[read_col]].row = read_row;
        LowerGraph[read_col][lowerColumnIndex[read_col]].col = read_col;
        lowerColumnIndex[read_col]++;
        
        // printf("LowerGraph[%d][%d].col = %d\n", read_row, lowerColumnIndex[read_row] - 1, LowerGraph[read_row][lowerColumnIndex[read_row] - 1].col);

        UpperGraph[read_row][upperColumnIndex[read_row]].row = read_col;
        UpperGraph[read_row][upperColumnIndex[read_row]].col = read_row;
        upperColumnIndex[read_row]++;

        // printf("UpperGraph[%d][%d].col = %d\n", read_col, upperColumnIndex[read_col] - 1, UpperGraph[read_col][upperColumnIndex[read_col] - 1].col);

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
   
   //arrays that stores the non zero elements of each row of the graph
    int *elemPerRowDown = (int *)calloc(nrows, sizeof(int)); 
    int *elemPerRowUp = (int *)calloc(nrows, sizeof(int));
    
    
    countElemPerRow(initialFile, elemPerRowDown, elemPerRowUp, nz);  //Creating the elemPerRowDown,elemPerRowUp matrices
    
    ElementsOfGraph **LowerGraph = allocateGraph(nrows , elemPerRowDown);
    ElementsOfGraph **UpperGraph = allocateGraph(nrows , elemPerRowUp);

    createGraph(initialFile, LowerGraph, UpperGraph, nz, nrows); 

    //int row_index = 0;
    int col_index = 0;
    csrMatrix->rows[0] = 0;

    //finding the row and col vectors that describe the Csr Matrix
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

    
    for(int i = 0; i < nrows; i++){
        if (elemPerRowDown[i] != 0) {
            free(LowerGraph[i]);
        }

        if (elemPerRowUp[i] != 0) {
            free(UpperGraph[i]);
        }
    }

    free(LowerGraph);
    free(UpperGraph);


    free(elemPerRowDown);
    free(elemPerRowUp);
}


void createCscMatrix(CSC *cscMatrix, FILE  *initialFile, int nz, int ncols){      
   
   //arrays that stores the non zero elements of each row of the graph
    int *elemPerColDown = (int *)calloc(ncols, sizeof(int)); 
    int *elemPerColUp = (int *)calloc(ncols, sizeof(int));
    
    
    countElemPerCol(initialFile, elemPerColDown, elemPerColUp, nz);  //Creating the elemPerRowDown,elemPerRowUp matrices
    
    ElementsOfGraph **LowerGraph = allocateGraph(ncols , elemPerColDown);
    ElementsOfGraph **UpperGraph = allocateGraph(ncols , elemPerColUp);

    creatGraphCol(initialFile, LowerGraph, UpperGraph, nz, ncols); 

    //int row_index = 0;
    int col_index = 0;
    cscMatrix->cols[0] = 0;

    //finding the row and col vectors that describe the Csr Matrix
    for(int col = 0 ; col < ncols; col++){
        for (int row = 0; row < elemPerColUp[col]; row++) {
            cscMatrix->rows[col_index] = UpperGraph[col][row].row;
            col_index++;
        }

        for(int row = 0; row < elemPerColDown[col]; row++){
            cscMatrix->rows[col_index] = LowerGraph[col][row].row;
            col_index++;
        }

        cscMatrix->cols[col + 1] = elemPerColDown[col] + elemPerColUp[col];
        cscMatrix->cols[col + 1] += cscMatrix->cols[col];
    }
    
    for(int i = 0; i < ncols; i++){
        if (elemPerColDown[i] != 0) {
            free(LowerGraph[i]);
        }
        if (elemPerColUp[i] != 0) {
            free(UpperGraph[i]);
        }
    }

    free(LowerGraph);
    free(UpperGraph);


    free(elemPerColDown);
    free(elemPerColUp);

}


/**
 * @brief  Prints the Csr Matrix 
 * 
 * @param csrMatrix 
 * @param nrows 
 */
void printCsrMatrix(CSR *csrMatrix) {
    
    printf("Columns: ");
    
    for (int i = 0; i < csrMatrix->nz; i++) {
        printf("%d ", csrMatrix->cols[i]);
    }
    
    printf("\n");

    printf("Indexes: ");
    
    for (int i = 0; i < csrMatrix->nrows + 1; i++) {
        printf("%d ", csrMatrix->rows[i]);
    }
    
    printf("\n");
}


void printCscMatrix(CSC *cscMatrix) {
    
    printf("Rows: ");
    
    for (int i = 0; i <  cscMatrix->nz; i++) {
        printf("%d ", cscMatrix->rows[i]);
    }
    
    printf("\n");

    printf("Indexes: ");
    
    for (int i = 0; i < cscMatrix->ncols + 1; i++) {
        printf("%d ", cscMatrix->cols[i]);
    }
    
    printf("\n");
}


/**
 * @brief Prints the Graph
 * 
 * @param nrows number of rows of the mtx fil
 * @param graph 
 * @param elemPerRow number of non zero elements of each row of the graph
 */
void printGraph(int nrows, int **graph, int *elemPerRow) {

    for (int i = 0; i < nrows; i++) {
        printf("Row %d: ", i + 1);
        for (int j = 0; j < elemPerRow[i]; j++) {
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }
}


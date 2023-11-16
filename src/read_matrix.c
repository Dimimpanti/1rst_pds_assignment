#include "read_matrix.h"
#include <stdlib.h>
#include "configuration_matrix.h"

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
    
    printf("%d %d %d\n", *nrows, *ncols, *nz);

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

//swap rows and cols in the mtx file
void reverseFile(FILE *initialFile){
    rewind(initialFile);
        
    int nrows, ncols , nz;

    readHeader(&nrows, &ncols, &nz, initialFile);

    FILE *invertedFile;
    invertedFile = fopen ("InvertedFile", "w");
    
    if (invertedFile == NULL){
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }

    // Write header of the inverted file
    fprintf(invertedFile,"%%MatrixMarket matrix coordinate pattern symmetric\n");
    fprintf(invertedFile, "%d %d %d\n", ncols, nrows, nz);

    int row, col;

    for(int i = 0; i < nz; i++){
        
        fscanf(initialFile, "%d %d ", &row, &col);
        
        fprintf(invertedFile, "%d %d\n", col, row);

    }

    fclose(invertedFile);
}


//counting the non-zero elements of the array A
void countElemPerRow(FILE *initialFile, int *elemPerRowDown, int *elemPerRowUp, int nz){
    
    rewind(initialFile);
        
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
 * @brief Create a Csr Matrix matrix from a file
 * 
 * @param rows rows vector of csr matrix
 * @param cols cols vector of csr matrix
 * @param fptr file pointer
 * @param nz number of nz elements
 */
void createCsrMatrix(int *csrRows, int *csrCols, FILE  *initialFile, int nz, int nrows){   
    
    // FILE *invertedFile;
    // invertedFile = fopen ("InvertedFile", "r");
    
    // if (invertedFile == NULL){
    //     printf("Error! opening file");
    //     // Program exits if file pointer returns NULL.
    //     return 1;
    // }

    // int nrows, ncols;
    // readHeader(&nrows, &ncols, &nz, invertedFile);
   
   
    int *elemPerRowDown = (int *)calloc(nrows, sizeof(int)); 
    int *elemPerRowUp = (int *)calloc(nrows, sizeof(int));
    
    
    
    countElemPerRow(initialFile, elemPerRowDown, elemPerRowUp, nz);//Creating the U,D matrices
    
    printf("D= ");
    for(int i = 0; i < nrows; i++){
        printf("%d ", elemPerRowDown[i]);
    }
    printf("\n");

    printf("U= ");
    for(int i = 0; i < nrows; i++){
        printf("%d ", elemPerRowUp[i]);
    }   
    printf("\n");

    rewind(initialFile);
    skipHeader(initialFile);

    int row, col;

    int last_row = -1;
    int row_index = 0;//iterates through the values of the matrix
   
    for(int i = 0; i < nz; i++){
        
        fscanf(initialFile, "%d %d ", &col, &row);
        
        csrCols[i] = col - 1;

        if(row - 1 != last_row){

            csrRows[row_index] = i;
            row_index++;
            last_row = row - 1;
        }

    }
   
}


// void printMatrixΩ(int *matrix, int rows, int cols) {
//     for (int i = 0; i < rows; i++) {
//         for (int j = 0; j < cols; j++) {
//             printf("%d ", matrix[i * cols + j]);
//         }
//         printf("\n");
//     }
// }



// void CsrMatrixΩ(int *MatrΩ, int nrows, int clusters) {
//     // Count the number of non-zero elements (nnz)
//     int nnz = 0;
//     for (int i = 0; i < nrows * clusters; i++) {
//         if (MatrΩ[i] != 0) {
//             nnz++;
//         }
//     }

//     // Allocate memory for CSR arrays
//     int *csrRows = (int *)calloc(nrows, sizeof(int));
//     int *csrCols = (int *)calloc(nnz , sizeof(int));

//     // Fill CSR arrays

//     int row_index = 0; //iritates trough the matrix
//     for (int i = 0; i < nrows; i++) {
//         for (int j = 0; j < clusters; j++) {
//             if (MatrΩ[i] != 0) {
//                 csrCols[row_index] = j;
//                 csrRows[i + 1]++;//only one non zero value per row 
//                 row_index++;
//             }
//         }
//         csrRows[i+1] += csrRows[i];
//     }


//     // Print the CSR matrix
//     printf("\nCSR MatrixΩ:\n");
//     printf("Csr Rows:");
//     for (int i = 0; i <= nrows; i++) {
//         printf("%d ", csrRows[i]);
//     }
//     printf("\n");
//     printf("Csr Cols:");
//     for (int i = 0; i < nnz; i++) {
//         printf("%d ", csrCols[i]);
//     }
//     printf("\n");}



// //random choice of clusters
// void MatrixΩ(int clusters, int nrows, FILE *initialFile) {
//     rewind(initialFile);
//     skipHeader(initialFile);

//     int *MatrΩ = (int *)malloc(nrows * clusters * sizeof(int));

//     for (int i = 0; i < nrows; i++) {
//         int nonZeroCol = rand() % clusters;

//         for (int j = 0; j < clusters; j++) {
//             MatrΩ[i * clusters + j] = 0;
//         }

//         MatrΩ[i * clusters + nonZeroCol] = 1;
//     }

//     // Print the matrix
//     printf("\n MatrixΩ:\n");
//     printMatrixΩ(MatrΩ, nrows, clusters);
//     CsrMatrixΩ(MatrΩ , nrows , clusters);
 
// }

int main(){
    
    FILE *fptr;
    fptr = fopen ("./../graphs/test_matrices/small_graph_2.mtx" , "r");
    if (fptr == NULL){
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        return 1;
    }

    int nrows, ncols , nz;    
    readHeader(&nrows, &ncols, &nz, fptr);
    
    int *csrCols = (int *)malloc(nz * sizeof(int));
    int *csrRows = (int *)malloc(nrows * sizeof(int));

    createCsrMatrix(csrRows, csrCols, fptr, nz, nrows);
    
    for(int i = 0; i < nz; i++){
        printf("%d ", csrRows[i]);
    }
    printf("\n");
    for(int i = 0; i < nz; i++){
        printf("%d ", csrCols[i]);
    }
    
    int clusters = 6;  // desired number of clusters
   

    MatrixΩ(clusters, nrows, fptr);


    fclose(fptr); 

    free(csrCols);
    free(csrRows);
    

    return 0;
}

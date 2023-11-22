#include <stdlib.h>

#include "read_matrix.h"
#include "configuration_matrix.h"


/**
 * @brief Finds the number of clusters from the file that contains the clusters of the nodes
 * 
 * @param fp    The file that contains the clusters of the nodes
 * @return int  The number of clusters
 */
int findNumberOfClusters(FILE *fp){
    int nClusters = 0;
    int cluster;

    // Read one by one the clusters from the file and find the maximum cluster
    while (fscanf(fp, "%d", &cluster) != EOF) {
        if (cluster > nClusters) {
            nClusters = cluster;
        }
    }

    return nClusters;
}


void csrConfigurationMatrix(FILE *clusteringFile, CSR *csrMatrix, int nNodes){

    int nClusters = findNumberOfClusters(clusteringFile);  // The number of clusters

    csrMatrix->nrows = nNodes;     // The rows of the matrix are the number of nodes
    csrMatrix->ncols = nClusters;  // The columns of the matrix are the number of clusters
    csrMatrix->nz = nNodes;        // The number of non-zero elements is equal to the number of nodes

    // Allocate memory for the arrays of the configuration matrix
    csrMatrix->rows = (int *)malloc((csrMatrix->nrows + 1) * sizeof(int));  //TODO : free done
    csrMatrix->cols = (int *)malloc(csrMatrix->nz * sizeof(int));           //TODO : free done
    csrMatrix->values = (int *)malloc(csrMatrix->nz * sizeof(int));         //TODO : free done


    // rewind the file to the beginning
    fseek(clusteringFile, 0, SEEK_SET);

    int rowIndex = 0;
    int cluster;

    for (int node = 0; node < nNodes; node++) {

        fscanf(clusteringFile, "%d", &cluster);

        // rebasing the cluster indexes from 1 to 0
        cluster--;

        csrMatrix->rows[rowIndex] = node;
        csrMatrix->cols[rowIndex] = cluster;
        csrMatrix->values[rowIndex] = 1;

        rowIndex++;
    }

    csrMatrix->rows[rowIndex] = nNodes;
}


/**
 * @brief Generates the configuration matrix of a graph in CSC format
 * 
 * @param nclusters  The number of clusters
 * @param csrMatrix  The CSR matrix
 */
void configurationMatrix(FILE *clusteringFile, CSC *cscMatrix, int nNodes){
    


    int nClusters = findNumberOfClusters(clusteringFile);  // The number of clusters

    cscMatrix->ncols = nClusters;  // The columns of the matrix are the number of clusters
    cscMatrix->nrows = nNodes;     // The rows of the matrix are the number of nodes
    cscMatrix->nz = nNodes;        // The number of non-zero elements is equal to the number of nodes

    // Allocate memory for the arrays of the configuration matrix
    cscMatrix->cols = (int *)malloc((cscMatrix->ncols + 1) * sizeof(int));  //TODO : free done
    cscMatrix->rows = (int *)malloc(cscMatrix->nz * sizeof(int));           //TODO : free done
    cscMatrix->values = (int *)malloc(cscMatrix->nz * sizeof(int));         //TODO : free done


    // The file is stored in CSR format but we need the matrix in CSC format
    // for this reason we can not directly read the file.

    int avnNodesPerCluster = nNodes / nClusters;  // The average number of nodes per cluster

    int **clusterCols = (int **)malloc(nClusters * sizeof(int *));  //TODO : free done

    for (int i = 0; i < nClusters; i++) {
        // Allocate memory for the array of the cluster. The size of the allocation is approximately twice 
        // the average number of nodes per cluster. This is done to avoid reallocating memory for every cluster.
        clusterCols[i] = (int *)malloc(2 * avnNodesPerCluster * sizeof(int));  //TODO : free done
    }

    int *clusterIndexes = (int *)calloc(nClusters, sizeof(int));  //TODO : free done

    // rewind the file to the beginning
    fseek(clusteringFile, 0, SEEK_SET);

    for (int node = 0; node < nNodes; node++) {
        int cluster;

        fscanf(clusteringFile, "%d", &cluster);

        // rebasing the cluster indexes from 1 to 0
        cluster--;


        // If the array of the cluster is full, reallocate memory for the array of the cluster
        if(clusterIndexes[cluster] % avnNodesPerCluster == 0 && clusterIndexes[cluster] != 0 && clusterIndexes[cluster] != avnNodesPerCluster){
            // Reallocate memory for the array of the cluster
            clusterCols[cluster] = (int *)realloc(clusterCols[cluster], (clusterIndexes[cluster] + avnNodesPerCluster) * sizeof(int));  //TODO : free done
        }


        // Add the node to the array of the cluster
        clusterCols[cluster][clusterIndexes[cluster]] = node;
        clusterIndexes[cluster]++;
    }


    // save the cluster columns in the configuration matrix
    int index = 0;

    for (int cluster = 0; cluster < nClusters; cluster++) {
        cscMatrix->cols[cluster] = index;

        for (int row = 0; row < clusterIndexes[cluster]; row++) {
            cscMatrix->rows[index] = clusterCols[cluster][row];
            cscMatrix->values[index] = 1;
            index++;
        }

        // free the array of the cluster
        free(clusterCols[cluster]);
    }

    cscMatrix->cols[nClusters] = index;

    free(clusterCols);
    free(clusterIndexes);
}


/**
 * @brief Given a CSC matrix, this function creates a CSR matrix that is the transpose of the CSC matrix
 * 
 * @param input   The CSC matrix
 * @param output  The transposed matrix in CSR format
 */
void transposeConfigurationMatrix(CSC *input , CSR *output){
    
    output->nz = input->nz;  // The number of non-zero elements is equal to the number of non-zero elements of the input matrix
    output->nrows = input->ncols;  // The number of rows is equal to the number of columns of the input matrix
    output->ncols = input->nrows;  // The number of columns is equal to the number of rows of the input matrix

    // Allocate memory for the arrays of the output matrix
    output->cols = (int *)malloc(input->nz * sizeof(int));           //TODO : free done
    output->rows = (int *)malloc((input->ncols + 1) * sizeof(int));  //TODO : free done
    output->values = (int *)malloc(input->nz * sizeof(int));         //TODO : free done


    for (int i = 0; i < input->nz; i++) {
        output->cols[i] = input->rows[i];
        output->values[i] = input->values[i];
    }
    

    for (int i = 0; i < input->ncols + 1; i++) {
        output->rows[i] = input->cols[i];
    }
}
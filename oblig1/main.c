#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "src/read_graph_from_file.h"


void main(){

	//Why pass N?
	int N;
	double *val;
	int *col_idx; 
    int *row_ptr;
	//read_graph_from_file(char *filename, int *N, int **row_ptr, int **col_idx, double **val)
	read_graph_from_file("src/example_data.txt", &N, &row_ptr, &col_idx, &val);

	printf("\n From main \n row_ptr: ");
    for(int i=0;i<=N-1;i++){
        printf(" %d ",row_ptr[i]); //segmantation memory no longer exists
    }
    printf("\n");
}
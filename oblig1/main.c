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

	read_graph_from_file("src/example_data.txt", &N, &row_ptr, &col_idx, &val);
	printf("Links: %d \n",row_ptr[N]);


	printf("from main\n");
	printf("\n A: ");
    for(int i=0;i<=row_ptr[N]-1;i++){//edges variable
        printf(" %f ",val[i]);
    }
    printf("\n col idx: ");
    for(int i=0;i<=row_ptr[N]-1;i++){
        printf(" %d ",col_idx[i]);
    }
    printf("\n row_ptr: ");
    for(int i=0;i<=N;i++){
        printf(" %d ",row_ptr[i]);
    }
    printf("\n");

	double d=0.85;
	double epsilon,*scores; 
	PageRank_iterations (N, row_ptr, col_idx,val,d,epsilon,scores);
	

}
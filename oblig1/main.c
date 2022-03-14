#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "src/read_graph_from_file.h"


void main(){

	//Why pass N?
	int N, Links;
	double *val;
	int *col_idx; 
    int *row_ptr;
	/* Reason for including Links variable:
	I need Links to know the length of val and col_idx. 
	As the both are dynamically allocates sizeof can't be used. 
	As Malloc does not guarantee that values are zero, i can't use this to
	find the lenght of the two arrays either. As a resault i need links. 
	*/
	read_graph_from_file("src/example_data.txt", &N, &Links, &row_ptr, &col_idx, &val);
	printf("Links: %d \n",Links);


	printf("from main\n");
	printf("\n A: ");
    for(int i=0;i<=Links-1;i++){//edges variable
        printf(" %f ",val[i]);
    }
    printf("\n col idx: ");
    for(int i=0;i<=Links-1;i++){
        printf(" %d ",col_idx[i]);
    }
    printf("\n row_ptr: ");
    for(int i=0;i<=N-1;i++){
        printf(" %d ",row_ptr[i]);
    }
    printf("\n");

	double d=0.85;
	double epsilon,*scores; 
	PageRank_iterations (N, Links, row_ptr, col_idx,val,d,epsilon,scores);
	

}
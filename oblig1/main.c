#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <omp.h>
#include "src/read_graph_from_file.h"


void main(){

	int N;
	double *val;
	int *col_idx; 
    int *row_ptr;

	read_graph_from_file("datafiles/100nodes.txt", &N, &row_ptr, &col_idx, &val); //Change filename to 100nodes.txt if needed

    printf("\nCol_idx: ");
    for(int i=0;i<=row_ptr[N]-1;i++){printf(" %d ",col_idx[i]);}

	printf("\nRow_ptr ");
    for(int i=0;i<=N;i++){printf(" %d ",row_ptr[i]);}


	double d=0.85;
	double epsilon=10e-7;
	double *scores = malloc(N*sizeof(double));

	PageRank_iterations (N, row_ptr, col_idx, val,d,epsilon,scores);



	
    int n = 3;	//The top scores to print
	top_n_webpages(N, scores, n);

}
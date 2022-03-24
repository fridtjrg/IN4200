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

	read_graph_from_file("datafiles/17nodes.txt", &N, &row_ptr, &col_idx, &val);
	printf("\n \n*read_graph_from_file has run* \n\n");



	double d=1;
	double epsilon=0.0001;
	double *scores = malloc(N*sizeof(double));

	PageRank_iterations (N, row_ptr, col_idx, val,d,epsilon,scores);
	printf("\n\n*PageRank_iterations has run*\n\n");

	/*
	printf("Final scores from main= ");
	for(int i=0; i<=N-1;i++){
            printf(" %f ",scores[i]);
        }
    */
	
    int n = 8;
	top_n_webpages(N, scores, n);
	printf("\n\n*top_n_webpages has run*\n\n");

}
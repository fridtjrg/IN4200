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

	read_graph_from_file("datafiles/17nodes.txt", &N, &row_ptr, &col_idx, &val); //Change filename to 100nodes.txt if needed


	double d=1;
	double epsilon=10e-7;
	double *scores = malloc(N*sizeof(double));

	PageRank_iterations (N, row_ptr, col_idx, val,d,epsilon,scores);



	
    int n = 3;	//The top scores to print
	top_n_webpages(N, scores, n);

}
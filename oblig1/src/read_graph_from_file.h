#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

void read_graph_from_file(char *filename, int *N, int *Links, int **row_ptr, int **col_idx, double **val);
void PageRank_iterations (int N, int Links, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores);
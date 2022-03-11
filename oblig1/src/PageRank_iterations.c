

void PageRank_iterations (int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores){
    //why is this not importing A???
    //initial guess
    float *x = malloc(N * sizeof(float));

    //Initial danglin webpage score
    float W = 1/N;

    //using for to parallellize, use while when approacing limit?
    for(int i=0;i <=N;i++){
        //The CRS multiplication is not correct
        x[i] = ((1-d+d*W)/N)+d*A_datapoints[col_idx[i]+N*row_ptr[i]]*x[i];

    }

}
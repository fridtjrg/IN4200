#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>

void PageRank_iterations (int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores){

    //initial guess
    double *x_old = malloc(N * sizeof(double));
    double *x_new = malloc(N * sizeof(double));


    for(int i=0;i<N;i++){
        x_old[i]=1./N;
        x_new[i]=0;
    }




    //These loops find the number of danling webpages and save their index.
    int num_W = 0; //number of dangling webpages
    int *W_webpages = malloc(N * sizeof(int));
    

    for(int i=0;i<=N-1;i++){W_webpages[i] = 0;}

    //webpages with a link gets value 1.
    for(int i=0;i<=row_ptr[N]-1;i++){W_webpages[col_idx[i]]= 1;}

    for(int i=0;i<=N-1;i++){
        if(W_webpages[i]==0){
            W_webpages[num_W]=i;   //saves index of dangling page in existing array
            num_W += 1;
        }}


    
    //saves the index of danling webpages in array
    int *Dangling_idx= malloc(num_W* sizeof(int));
    for(int i=0;i<=num_W-1;i++){
        Dangling_idx[i] = W_webpages[i];
        //printf("\n i=%d index_saved: %d ",i, W_webpages[i]);
    }
    

    free(W_webpages); 
    if(num_W==0){free(Dangling_idx);} //if there are no danling webpages

    

    //Initial danglin webpage score
    double W = 1/N;
    if(num_W==0){W=0;}

    double test_criterion= epsilon+1; 
    //using for to parallellize, use while when approacing limit?


    while(epsilon<=test_criterion){ //for each iteration

        double per_iter = ((1-d+d*W)/N); //needs only be calculated once per iteration

        //CRS multiplication
        int n_multip;
        for (int i=0;i<=N-2; i++){
            n_multip = row_ptr[i+1]- row_ptr[i]-1; //nr multiplications in row
            x_new[i] = 0;
            for (int j=0; j<=n_multip;j++){
                x_new[i]+=val[row_ptr[i]+j]*x_old[col_idx[row_ptr[i]+j]];
            }

            x_new[i] = per_iter+ d*x_new[i];

        }


        
        //calculates last value of x array
        double temp=0;
        for (int j=0; j<=row_ptr[N]-row_ptr[N-1];j++)
            {
                temp+=val[row_ptr[N-1]+j]*x_old[col_idx[row_ptr[N-1]+j]];
            }
        x_new[N-1] = per_iter+d*temp;   

        //sum scores of dandling webpages
        W=0;
        for(int i=0;i<num_W-1;i++){
            W+= x_new[Dangling_idx[i]];//sum scores of dangling webpages
        }
        
        test_criterion = 0;
        
        //updates old x to be new x and updates test criterion
        for(int i=0;i<=N-1;i++){
            test_criterion += fabs(x_new[i]-x_old[i]);
            x_old[i] = x_new[i];
        }

}//end of iteration loop


}//end of function


#include "PageRank.h"

void PageRank_iterations (int N, int *row_ptr, int *col_idx, double *val, double d, double epsilon, double *scores){

    //Global public variables (Note that scores is used instead of x_new)
    double *x_old = malloc(N * sizeof(double));
    int num_W = 0;  //number of dangling webpages
    double W=0;     //Summation of pagereank scores   
    int *W_webpages = malloc(N * sizeof(int));
    int *Dangling_idx;

    #pragma omp parallel
    {

    //Sets values in arrays
    #pragma omp for
    for(int i=0;i<N;i++){
        x_old[i]=1./N;
        W_webpages[i] = 0;
    }


    #pragma omp barrier
    
    //only webpages with a link gets value 1.
    #pragma omp single
    {
    for(int i=0;i<=row_ptr[N]-1;i++){W_webpages[col_idx[i]]= 1;}
    }//implicit barrier
    
    //Counts the amount of dangling webpages and saves their indeces
    #pragma omp single
    {
    for(int i=0;i<N;i++){
        if(W_webpages[i]==0){
            W_webpages[num_W]=i;   
            num_W += 1;
        }}

    Dangling_idx= malloc(num_W* sizeof(int));
    }//implicit barrier


    //saves the index of danling webpages in new array appropriately sized array and sets value of the W value
    #pragma omp for reduction(+:W)
    for(int i=0;i<num_W;i++){
        Dangling_idx[i] = W_webpages[i];
        W += 1./N;
    }
    
    
    }//end of parallel region

    //Frees arrays that are not usefull.
    free(W_webpages); 
    if(num_W==0){free(Dangling_idx);} //if there are no danling webpages



    //guarantes the while loop starts
    double test_criterion= epsilon+1.; 
    double temp;



    #pragma omp parallel
    {
    while(epsilon<=test_criterion){ //for each iteration
        double per_iter = ((1.-d+d*W)/N); //needs only be calculated once per iteration


        //CRS multiplication
        #pragma omp for
        for (int i=0;i<=N-2; i++){
            //if the row is not empty
            if(row_ptr[i]>=0){

                int next_idx;
                //finds index of next row which is not empty
                for(int k=i; k<=N-2;k++){
                    if(row_ptr[k+1]!=-1){
                        next_idx = k+1;
                        break;
                    }

                }
                int n_multip = row_ptr[next_idx]- row_ptr[i]; //nr multiplications in row
                scores[i] = 0;
                //Calculates score for a given node.
                for (int j=0; j<n_multip;j++){
                    scores[i]+=val[row_ptr[i]+j]*x_old[col_idx[row_ptr[i]+j]];
                }
            }
            //If the webpage is dangling
            if(row_ptr[i]==-1){
                scores[i]=0;
            }

            //Final score for a given iteration is calculated
            scores[i] = per_iter+ d*scores[i];

        }


        
        //calculates last value of x array
        #pragma omp single
        {        
            temp=0.;
        }

        //If last webpage is NOT dangling
        if(row_ptr[N-1]>=0){
            #pragma omp for reduction(+:temp)
            for (int j=0; j<=row_ptr[N]-row_ptr[N-1];j++)
                {
                    temp+=val[row_ptr[N-1]+j]*x_old[col_idx[row_ptr[N-1]+j]];
                }
        }



          

        //sum scores of dandling webpages
        #pragma omp single
        {
            //if webage is danling
            if(row_ptr[N-1]==-1){
                temp=0.;
            }
            scores[N-1] = per_iter+d*temp; //avoids race condition
            W=0.;
        }//implicit barrier


        //Calulates the W to use in next iteration
        #pragma omp for reduction(+:W)
        for(int i=0;i<num_W;i++){
            W+= scores[Dangling_idx[i]];//sum scores of dangling webpages
        }



        
        
        #pragma omp single
        {
            test_criterion = 0;     //Avoids race condition
        }//implicit barrier
        
        //updates old x to be scores and updates test criterion
        #pragma omp for reduction(+:test_criterion)
        for(int i=0;i<=N-1;i++){
            test_criterion += fabs(scores[i]-x_old[i]);
            x_old[i] = scores[i];
        }

        
        
        

}//end of iteration loop
}//end of parallell region
}//end of function


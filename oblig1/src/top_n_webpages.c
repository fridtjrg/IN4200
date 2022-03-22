#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void top_n_webpages(int N, double *scores, int n){

	double *top_scores = malloc(n*sizeof(int));
	int *top_scores_idx = malloc(n*sizeof(int));




	top_scores_idx[0] = 0;

	//Finds index of top score
	for(int i=0; i<=N; i++){
		if(scores[i]>scores[top_scores_idx[0]]){top_scores_idx[0]=i;}
	}

	//saves best value and zeros that value in scores list
	top_scores[0] = scores[top_scores_idx[0]];
	scores[top_scores_idx[0]] = 0;

	int num_threads;
	double *best_candidates;
	int *best_candidates_idx;


	#pragma omp parallel //Only to create the thread_num dependent variables
	{
		#pragma omp single
		{
			num_threads = omp_get_num_threads();
			printf("\nnum thread = %d",num_threads);
			best_candidates = malloc(num_threads*sizeof(double));
			best_candidates_idx = malloc(num_threads*sizeof(int));
		}


		
	}
	
		for(int idx=1; idx<=n-1; idx++){
		#pragma omp parallel
		{
		int thread_id = omp_get_thread_num();
			#pragma omp for
			for(int i=0; i<=N; i++){
				if(scores[i]>best_candidates[thread_id]){
					best_candidates[thread_id]=scores[i];
					best_candidates_idx[thread_id]= i;
				}
			}
			//printf("\n thread: %d found %f",thread_id, best_candidates[thread_id]);

			#pragma omp barrier	//all threads must be done searching
			#pragma omp single	//Threads cannot find new candidate before the best score is chosen
			{
			double best_score = best_candidates[0]; //why semicolon?
			int best_score_idx = best_candidates_idx[0];

			for(int i =1; i<=num_threads;i++){
				if(best_candidates[i]>best_score){
					best_score = best_candidates[i];
					best_score_idx = best_candidates_idx[i];
				}
			}

			//printf("\n best score was %f \n",best_score);
			top_scores[idx] = best_score;
			top_scores_idx[idx] = best_score_idx;
			scores[best_score_idx]= 0; //zeros value so the value can not be picked again
			/*
			printf("\n current list");
			for(int j=0;j<=N;j++){
				printf(" %f ", scores[j]);
			}
			*/
			}
			//end of single region
			//resets each threads best candidate
			best_candidates[thread_id] = 0;


		}
	}

	//end of for parallel region

	//refills the zeroed values
	#pragma omp parallel for
	for(int i=0; i<=n-1;i++){
		scores[top_scores_idx[i]] = top_scores[i];
	}


	printf("\nWebsites ranks as follows:\n");
	for(int i=0; i<=n-1; i++){
		printf("%d. Score= %f, index = %d \n", i+1, top_scores[i],top_scores_idx[i]);
	}


}//end of function



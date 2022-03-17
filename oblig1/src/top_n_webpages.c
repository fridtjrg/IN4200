#include <stdio.h>
#include <stdlib.h>

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

	for(int idx=1; idx<=n-1; idx++){
		double top_score = 0;
		int top_score_idx;
		
		for(int i=0; i<=N; i++){
			if(scores[i]>top_score){

				top_score=scores[i];
				top_score_idx= i;
			}
		}

		scores[top_score_idx] = 0;		//zeros the value to avoid checking if value has already been zeroed
		top_scores[idx]= top_score;
		top_scores_idx[idx] = top_score_idx;

	}

	//refills the zeroed values
	for(int i=0; i<=n-1;i++){
		scores[top_scores_idx[i]] = top_scores[i];
	}


	printf("\nWebsites ranks as follows:\n");
	for(int i=0; i<=n-1; i++){
		printf("%d. Score= %f, index = %d \n", i+1, top_scores[i],top_scores_idx[i]);
	}


}//end of function



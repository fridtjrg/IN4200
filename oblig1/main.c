#include "src/main.h"



void main(int argc, char *argv[]){

	char *file_name;
	double d;
	double epsilon;
	int n;

	if(argc ==5){
		file_name = argv[1];
		d=atof(argv[2]);
		epsilon=atof(argv[3]);
		n = atoi(argv[4]);
	}
	else{
		printf("Arguments were not provided correctly,\n");
		printf("using default values!\n");

		file_name = "datafiles/100nodes.txt";
		d=0.85;
		epsilon= 10e-7;
		n = 3;
	}
	//Variables needed for read_graph_from_file()
	int N;
	double *val;
	int *col_idx; 
    int *row_ptr;

    


	read_graph_from_file(file_name, &N, &row_ptr, &col_idx, &val); //Change filename to 100nodes.txt if needed

	//Variables needed for PageRank_iterations()
	
	
	double *scores = malloc(N*sizeof(double));

	PageRank_iterations(N, row_ptr, col_idx, val,d,epsilon,scores);

	//The top scores to print
    	
	
	top_n_webpages(N, scores, n);
}
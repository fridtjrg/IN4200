
#include "read_graph_from_file.h"

//void read_graph_from_file(char *filename, int *N, int **row_ptr, int **col_idx, double **val) why N?
void read_graph_from_file(char *filename, int *N, int *Links, int **row_ptr_o, int **col_idx_o, double **val){

	FILE *datafile;
	datafile = fopen(filename, "r");
	int edges, nodes;
	int fromnode, tonode;

    if (datafile == NULL) {
        printf("Could not open %s\n", filename);
        exit(0);
    }
    
    //Skips first 4 lines
    fscanf(datafile, "%*[^\n]\n");
    fscanf(datafile, "%*[^\n]\n");
    fscanf(datafile, "%*s %*s %d %*s %d\n",&nodes, &edges);//saves number of edges
    *N = nodes;
    *Links = edges;
    fscanf(datafile, "%*[^\n]");

    //creates the full matrix
    double *A_temp = malloc(nodes*nodes*sizeof(double));

    //creates the wanted arrays
    double *A_datapoints_temp = malloc(edges*sizeof(double));
    int *col_idx = malloc(edges * sizeof(int));
    int *row_ptr = malloc(nodes * sizeof(int));

    for(int i=0;i<=edges-1;i++){
        A_datapoints_temp[i] = 0;
    }
    //Reads all datalines based on number of edges
    //Creates A, col_idx and row_idx
	for(int i=0;i<=edges-1;i++){  
        fscanf(datafile, "%d %d", &fromnode,&tonode);


    	A_temp[fromnode+nodes*tonode] = 1;
    	A_datapoints_temp[fromnode] += 1;

	}
    fclose(datafile);
    
    
    //Prints A_temp matrix
    for(int i=0;i<=nodes-1;i++){
    	for(int j=0;j<=nodes-1;j++){
    		printf(" %f ",A_temp[j+i*nodes]);
    }
    printf("\n");
    }
    

    //Calculates comung and row vector
    int temp_c=0;
    int temp_r=0;
    bool first_element;
    for(int i=0;i<=nodes-1;i++){
    	first_element = true;
    	
    	for(int j=0;j<=nodes-1;j++){
    		if(A_temp[j+i*nodes]!=0){
    			col_idx[temp_c] = j;

    			if(first_element==true)
    			{
    				row_ptr[temp_r] = temp_c;
    				temp_r+=1;
    				first_element = false;
    			}
    			temp_c += 1;
    		}
    }
    }
    free(A_temp);

    *val = malloc(edges*sizeof(double));
    for(int i=0;i<=edges-1;i++){
    	{
    		(*val)[i]= 1/A_datapoints_temp[col_idx[i]];
    	}  
    }
    free(A_datapoints_temp);


    /*
    printf("\n A: ");
    for(int i=0;i<=edges-1;i++){
        printf(" %f ",A_datapoints[i]);
    }

    printf("\n col idx: ");
    for(int i=0;i<=edges-1;i++){
        printf(" %d ",col_idx[i]);
    }
    printf("\n row_ptr: ");
    for(int i=0;i<=nodes-1;i++){
        printf(" %d ",row_ptr[i]);
    }
    printf("\n");
    */

    //"output"
    *row_ptr_o = row_ptr;
    *col_idx_o = col_idx;
}





#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void read_graph_from_file(char *filename);

//void read_graph_from_file(char *filename, int *N, int **row ptr, int **col idx, double **val)
void read_graph_from_file(char *filename){

	FILE *datafile;
	datafile = fopen(filename, "r");
	int V1, V2;

    if (datafile == NULL) {
        printf("Could not open %s\n", filename);
        exit(0);
    }
    
    //Skips first 4 lines
    fscanf(datafile, "%*[^\n]\n");
    fscanf(datafile, "%*[^\n]\n");
    fscanf(datafile, "%*[^\n]\n");
    fscanf(datafile, "%*[^\n]");

    //reads to EOF
	while(fgetc(datafile)!=EOF)
	{  
        fscanf(datafile, "%d %d", &V1,&V2);
    	printf("v1: %d, v2: %d\n", V1,V2);
	}


    fclose(datafile);
}

void read_graph_from_file(char *filename){

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
    fscanf(datafile, "%*[^\n]");

    float *A_datapoints = malloc(nodes * sizeof(float));
    int *col_idx = malloc(edges * sizeof(int));
    int *row_ptr = malloc(edges * sizeof(int));

    float count=0;
    //Reads all datalines based on number of edges
    //Creates A, col_idx and row_idx
    for(int i=0;i<=edges-1;i++){  
        fscanf(datafile, "%d %d", &fromnode,&tonode);

        col_idx[i] = fromnode;
        row_ptr[i] = tonode;

        if(fromnode==col_idx[i-1])
        {
            count+=1;
        }
        else{
            A_datapoints[col_idx[i-1]]= 1/count;
            count = 1;
        }
        if (i==edges-1)
        {
            A_datapoints[col_idx[i-1]] = 1/count;
        }
    }
    fclose(datafile);
    
    printf("\n col idx: ");
    for(int i=0;i<=edges-1;i++){
        printf(" %d ",col_idx[i]);
    }
    printf("\n row_ptr: ");
    for(int i=0;i<=edges-1;i++){
        printf(" %d ",row_ptr[i]);
    }
    printf("\n A: ");
    for(int i=0;i<=nodes-1;i++){
        printf(" %f ",A_datapoints[i]);
    }
}

void main(){
	read_graph_from_file("example_data.txt");
}


 /*
//From week1 exercise 2a solution
void readfile(char *filename, temperature_data *data) {

    int err;
    FILE *datafile;

    datafile = fopen(filename, "r");

    if (datafile == NULL) {
        printf("Failure to open file %s\n", filename);
        exit(0);
    }

    err = fscanf(datafile, "%d", &data->n);
    printf("err: %d \n", err);
    printf("Data struct: %d \n", data->n);

    data->times = (char *)malloc(5 * data->n * sizeof(char));
    data->temps = (double *)malloc(data->n * sizeof(double));

    for (int i = 0; i < data->n; i ++) {
        err = fscanf(datafile, "%5s %lf", &data->times[5 * i], &data->temps[i]);
    }

    fclose(datafile);
}
*/
#include "PageRank.h"

//Sorting function
int cmpfunc (const void * a, const void * b){
   return ( *(int*)a - *(int*)b );
}


void read_graph_from_file(char *filename, int *N, int **row_ptr, int **col_idx, double **val){

    FILE *datafile;
    datafile = fopen(filename, "r");
    int edges, nodes;   //"Edges" is the number of links
    int fromnode, tonode;

    if (datafile == NULL) {
        printf("Could not open %s\n", filename);
        exit(0);
    }
    
    //Skips first 4 lines
    fscanf(datafile, "%*[^\n]\n");
    fscanf(datafile, "%*[^\n]\n");
    fscanf(datafile, "%*s %*s %d %*s %d\n",&nodes, &edges);//saves number of nodes and edges
    *N = nodes;
    fscanf(datafile, "%*[^\n]");



    //creates the wanted arrays
    double *values_in_column = malloc(nodes*sizeof(double));
    *col_idx = malloc(edges * sizeof(int));
    *row_ptr = malloc((nodes+1) * sizeof(int));
    (*row_ptr)[nodes]=edges;

    int *fromnode_array = malloc(edges * sizeof(int));
    int *tonode_array = malloc(edges * sizeof(int));
    int *values_in_row = malloc(nodes * sizeof(int));

    for(int i=0;i<=nodes-1;i++){
        values_in_column[i] = 0;
    }
    //Reads all datalines based on number of edges
    //Saves tonode and fromnode as arrays
    //Saves number of values in each column and row
    for(int i=0;i<=edges-1;i++){  
        fscanf(datafile, "%d %d", &fromnode,&tonode);

        fromnode_array[i] = fromnode;
        tonode_array[i] = tonode;
        
        values_in_row[tonode] += 1;
        values_in_column[fromnode] += 1;

    }
    fclose(datafile);


    int current_col_idx = 0;
    int current_row_idx = 0;
    int storage_idx;
    int *temp_storage;
    int first_row_element; 
    int first_row_element_col_idx;
    
    //Calculates row ptr and col_idx
    for(int j=0; j<nodes; j++){

        storage_idx = 0;
        first_row_element = 0; 
        first_row_element_col_idx = nodes;
        temp_storage = malloc(values_in_row[j] * sizeof(int));
        
        //For each row the col_idx and row ptr is saved.
        for(int i =0; i<edges; i++){
            //if the tonode id is equal to what is being searched for, corresponding fromnode is saved in temporary array.
            if(tonode_array[i]==j){
                temp_storage[storage_idx] = fromnode_array[i];

                //If the column index is also the smallest in the current row
                if (fromnode_array[i]<first_row_element_col_idx){
                    first_row_element_col_idx = fromnode_array[i];
                    first_row_element = current_col_idx; 
                }
                storage_idx += 1;
            }
        }

        //Row element added only if row actually has value
        if(first_row_element_col_idx<nodes){
            (*row_ptr)[current_row_idx]=first_row_element; 
            current_row_idx += 1;
        }
        //If the row did not contain any values the row_ptr gets value -1 to signify an empty row.
        else{
            (*row_ptr)[current_row_idx]=-1; 
            current_row_idx += 1;
        }

        //The column indeces are sorted in ascending order and placed into col_idx array
        qsort(temp_storage, storage_idx, sizeof(int), cmpfunc);
        for(int i=0; i<storage_idx;i++){
            (*col_idx)[current_col_idx] = temp_storage[i];
            current_col_idx += 1;
        }

    }
    //free memory of arrays that are no longer usefull
    free(tonode_array);
    free(fromnode_array);
    free(values_in_row);
    //free(temp_storage); Why can't i free this?
    
    //Number of values in a column is used inverted to find appropriate value
    *val = malloc(edges*sizeof(double));
    for(int i=0;i<=edges-1;i++){
            (*val)[i]= 1/values_in_column[(*col_idx)[i]];  
    }
    free(values_in_column);
}

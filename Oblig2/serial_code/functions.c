#include <stdio.h>
#include <stdlib.h>


struct image {
float** image_data; /* a 2D array of floats */
int m; /* # pixels in vertical-direction */
int n; /* # pixels in horizontal-direction */
};


//Does it have to be a 2D array or can i just use..
//u = (float*)malloc(n*m*sizeof(float));
//to allocate?

//void allocate_image(image *u, int m, int n)
void allocate_image(*u, int m, int n){

	int* arr[m];
    for (int i = 0; i < m; i++)
        arr[i] = (int*)malloc(n * sizeof(int));
 

    int count = 0;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            arr[i][j] = ++count;
 
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            printf("%d ", arr[i][j]);

    //to free the memory
   /*
    for (int i = 0; i < m; i++)
        free(arr[i]);
    */
 
    return 0;
}

//How am i suppose to allocate 2D without m?

//void deallocate_image(image *u)
void deallocate_image(*u, int m){
	for (int i = 0; i < m; i++)
        free(u[i]);

    //if 1D just free(u)
}
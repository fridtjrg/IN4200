//Serial functions
#include <stdio.h>
#include <stdlib.h>


typedef struct{
float** image_data; /* a 2D array of floats */
int m; /* # pixels in vertical-direction */
int n; /* # pixels in horizontal-direction */
}image;



//Accocates image from struct
void allocate_image(image *u, int m, int n){
    u->m = m; u->n =n;

    u->image_data = (float**)malloc(m*n*sizeof(float*)); //was originally just m
    for (int i = 0; i < m; i++){
        u->image_data[i] = (float*)malloc(n * sizeof(float));
    }
}
    

//Frees image from struct
void deallocate_image(image *u){
    for (int i = 0; i < u->m; i++)
        free(u->image_data[i]);

    free(u->image_data);
}

//Assigns 1d image_chars array to a 2D float array in struct
void convert_jpeg_to_image(const unsigned char* image_chars, image *u){
    for(int i = 0; i < u->m; i++){
        for(int j=0; j < u->n; j++){
            u->image_data[i][j] = (float)image_chars[i*u->n+j];
        }
    }
}

//Assigns values from 2d float array in struct to 1d char array
void convert_image_to_jpeg(const image *u, unsigned char* image_chars){
    for(int i = 0; i < u->m; i++){
        for(int j=0; j < u->n; j++){
            image_chars[i*u->n+j] =(char)u->image_data[i][j];
        }
    }   
}

//Performs ISO diffusion denoising
void iso_diffusion_denoising(image *u, image *u_bar, float kappa, int iters){

    for(int iter=0; iter<iters; iter++){

        //As loop does not contain outer bound points
        //they should remain constant.
        for(int i = 1; i < u->m-1; i++){
            for(int j=1; j < u->n-1; j++){
                u_bar->image_data[i][j] = u->image_data[i][j] + kappa*(u->image_data[i-1][j] + u->image_data[i][j-1]-4*u->image_data[i][j] +u->image_data[i+1][j]+u->image_data[i][j+1]);
            }
        }

        //Swaps pointers only if it is NOT the last iteration
        if(iter!=iters-1){
            image temp = *u;
            *u = *u_bar;
            *u_bar = temp;
        }
    }
}





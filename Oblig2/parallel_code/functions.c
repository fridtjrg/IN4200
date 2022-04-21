//Parallel functions
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


typedef struct{
float** image_data; /* a 2D array of floats */
int m; /* # pixels in vertical-direction */
int n; /* # pixels in horizontal-direction */
}image;


//Does it have to be a 2D array or can i just use..
//u = (float*)malloc(n*m*sizeof(float));
//to allocate?

//void allocate_image(image *u, int m, int n)
void allocate_image(image *u, int m, int n){
    
    u->m = m; u->n =n;

	u->image_data = (float**)malloc(m*sizeof(float*)); 
    for (int i = 0; i < m; i++)
        u->image_data[i] = (float*)malloc(n * sizeof(float));
}
    


//void deallocate_image(image *u)
void deallocate_image(image *u){
	for (int i = 0; i < u->m; i++)
        free(u->image_data[i]);

    free(u->image_data);
}

void convert_jpeg_to_image(const unsigned char* image_chars, image *u){
//convert a 1D array of unsigned char values into an image struct

//NB: Assuming 1st element is top left and reading row-wise
    for(int i = 0; i < u->m; i++){
        for(int j=0; j < u->n; j++){
            u->image_data[i][j] = (float)image_chars[i*u->n+j];
        }
    }

}

void convert_image_to_jpeg(const image *u, unsigned char* image_chars){
    for(int i = 0; i < u->m; i++){
        for(int j=0; j < u->n; j++){
            image_chars[i*u->n+j] =(char)u->image_data[i][j];
        }
    }	
}

void iso_diffusion_denoising(image *u, image *u_bar, float kappa, int iters){

    for(int iter=0; iter<iters; iter++){

        //does not include the outer bounds
        for(int i = 1; i < u->m-1; i++){
            for(int j=1; j < u->n-1; j++){
                u_bar->image_data[i][j] = u->image_data[i][j] + kappa*(u->image_data[i-1][j] + u->image_data[i][j-1]-4*u->image_data[i][j] +u->image_data[i+1][j]+u->image_data[i][j+1]);
            }
        }

        //Swaps pointers only if it is not the last iteration
        if(iter!=iters-1){
            image temp = *u;
            *u = *u_bar;
            *u_bar = temp;
        }

    }//end of iteration loop


}

void iso_diffusion_denoising_parallel(image *u, image *u_bar, float kappa, int iters, int my_rank, int num_procs){
    //ghost points
    float* top_ghostpoints = malloc(u->n*sizeof(float));
    float* bottom_ghostpoints = malloc(u->n*sizeof(float));


    for(int iter=0; iter<iters; iter++){

        //exchaning ghost points
         if(num_procs > 1){
            if(my_rank == 0){//Rank 0 has no top ghostpoints as it contains the top of the image
                MPI_Send(u->image_data[u->m-1], u->n, MPI_FLOAT, my_rank + 1, 0, MPI_COMM_WORLD);
                MPI_Recv(bottom_ghostpoints, u->n, MPI_FLOAT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }

            //Even numbers recive first, odd send first. This avoids deadlock
            if((my_rank % 2) == 1 && my_rank != num_procs - 1 && my_rank != 0){
                MPI_Recv(top_ghostpoints, u->n, MPI_FLOAT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(u->image_data[u->m-1], u->n, MPI_FLOAT, my_rank + 1, 0, MPI_COMM_WORLD);
                MPI_Recv(bottom_ghostpoints, u->n, MPI_FLOAT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(u->image_data[0], u->n, MPI_FLOAT, my_rank - 1, 0, MPI_COMM_WORLD);
            }
            if((my_rank % 2) == 0 && my_rank != num_procs - 1 && my_rank != 0){
                MPI_Send(u->image_data[u->m-1], u->n, MPI_FLOAT, my_rank + 1, 0, MPI_COMM_WORLD);
                MPI_Recv(top_ghostpoints, u->n, MPI_FLOAT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(u->image_data[0], u->n, MPI_FLOAT, my_rank - 1, 0, MPI_COMM_WORLD);
                MPI_Recv(bottom_ghostpoints, u->n, MPI_FLOAT, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            //last process has no bottom ghostpoints, as it is the bottom of the image
            if(my_rank == num_procs - 1){
                MPI_Recv(top_ghostpoints, u->n, MPI_FLOAT, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(u->image_data[0], u->n, MPI_FLOAT, my_rank - 1, 0, MPI_COMM_WORLD);
            }
        }

        //does not include the outer bounds
        for(int i = 1; i < u->m-1; i++){
            for(int j=1; j < u->n-1; j++){
                u_bar->image_data[i][j] = u->image_data[i][j] + kappa*(u->image_data[i-1][j] + u->image_data[i][j-1]-4*u->image_data[i][j] +u->image_data[i+1][j]+u->image_data[i][j+1]);
            }
        }

        //Ghost points
        if(my_rank !=0 && my_rank != num_procs-1){
            for(int i=1;i< u->n-1;i++){
                //Top ghost points
                u_bar->image_data[0][i] = u->image_data[0][i] + kappa*(u->image_data[1][i] + u->image_data[0][i-1]-4*u->image_data[0][i] +top_ghostpoints[i]+u->image_data[0][i+1]);

                //Bottom ghost points
                u_bar->image_data[u->m-1][i] = u->image_data[u->m-1][i] + kappa*(bottom_ghostpoints[i] + u->image_data[u->m-1][i-1]-4*u->image_data[u->m-1][i]+ u->image_data[u->m-2][i] +u->image_data[u->m-1][i+1]);

            }
            
        }

        //Has only bottom ghost points
        if(my_rank==0){
            for(int i=1;i< u->n-1;i++){
            u_bar->image_data[u->m-1][i] = u->image_data[u->m-1][i] + kappa*(bottom_ghostpoints[i] + u->image_data[u->m-1][i-1]-4*u->image_data[u->m-1][i]+ u->image_data[u->m-2][i] +u->image_data[u->m-1][i+1]);
            }

        }

        //has only top ghost points
        if(my_rank==num_procs-1){
            for(int i=1;i< u->n-1;i++){
            u_bar->image_data[0][i] = u->image_data[0][i] + kappa*(u->image_data[1][i] + u->image_data[0][i-1]-4*u->image_data[0][i] +top_ghostpoints[i]+u->image_data[0][i+1]);
            }

        }

        //Swaps pointers only if it is not the last iteration
        if(iter!=iters-1){
            image temp = *u;
            *u = *u_bar;
            *u_bar = temp;
        }

    }//end of iteration loop

    free(top_ghostpoints);
    free(bottom_ghostpoints);

}



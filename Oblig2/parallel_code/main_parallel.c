#include "main_parallel.h"
#include <mpi.h>

void import_JPEG_file (const char* filename, unsigned char** image_chars,int* image_height,int* image_width,int* num_components);
void export_JPEG_file (const char* filename, const unsigned char* image_chars,int image_height, int image_width,int num_components, int quality);



int main(int argc, char *argv[])
{
	int m, n, c, iters;
	int my_m, my_n, my_rank, num_procs;
	float kappa;
	image u, u_bar, whole_image;
	unsigned char *image_chars;
	char *input_jpeg_filename, *output_jpeg_filename;

	//uses default values if no arguments are provided
	if(argc ==5){
		kappa = atof(argv[1]);
		iters=atof(argv[2]);
		input_jpeg_filename= argv[3];
		output_jpeg_filename = argv[4];
	}
	else{
		printf("Arguments were not provided correctly,\n");
		printf("using default values!\n");

		kappa = 0.1; //0.2 or lower
		iters= 100; //Many?
		input_jpeg_filename= "datafiles/mona_lisa_noisy.jpg";
		output_jpeg_filename = "datafiles/mona_lisa_parallel_processed.jpg";
	}

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	//Process 0 creates creates image_chars and allocates whole image
	if (my_rank==0) {
		import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
		allocate_image (&whole_image, m, n);
	}

	//m and n are broadcasted to all processes
	MPI_Bcast (&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast (&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	/* 
	--2D decomposition of the m x n pixels--
	Each process handles a vertical fraction of the image.(A certain number of rows)
	Numer of rows per process is simply the number of rows divided by the 
	number of processed and rounded down. If the number of rows can not
	be divided by the number of processes the last process will calculate
	the remaining rows.
	*/

	int average_chunksize = floor(m/num_procs);


	my_m = (my_rank+1)*average_chunksize - my_rank*average_chunksize;
	my_n = n;

	//Last process implicitly will complete the "rest"
	if(my_rank == num_procs-1){
		my_m = m - my_rank*average_chunksize;
	}
	
	int process_chunk_size = my_m*my_n;


	//Contains the chunk size of each process
	int *process_chunk_sizes = malloc(num_procs*sizeof(*process_chunk_sizes));
	MPI_Gather(&process_chunk_size, 1, MPI_INT, &process_chunk_sizes[my_rank], 1, MPI_INT, 0, MPI_COMM_WORLD);

	//Contains what row each chunk starts at
	int my_row_skip=average_chunksize*my_rank;
	int *process_row_skips = malloc(num_procs*sizeof(*process_row_skips));
	MPI_Gather(&my_row_skip, 1, MPI_INT, &process_row_skips[my_rank], 1, MPI_INT, 0, MPI_COMM_WORLD);


	//Contains the index of each chunks first element in image_chars array
	int *chunk_idx = malloc(num_procs*sizeof(*chunk_idx));
    for(int proc = 0; proc < num_procs; proc++){
        chunk_idx[proc] = proc*process_chunk_sizes[proc];
    }

    //Each process recives it's part of the image(image_chars)
    unsigned char *my_image_chars = malloc(process_chunk_size*sizeof(my_image_chars));
	MPI_Scatterv(image_chars, process_chunk_sizes, chunk_idx, MPI_UNSIGNED_CHAR, my_image_chars, process_chunk_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
	free(process_chunk_sizes);
	free(chunk_idx);


	//Allocates memory for this processes' calculation
    allocate_image(&u, my_m, my_n);
    allocate_image(&u_bar, my_m, my_n);

    //Converts 1d char array to struct
	convert_jpeg_to_image(my_image_chars, &u);
	free(my_image_chars);

	//Performs ISO diffusion denoising in parallel
	iso_diffusion_denoising_parallel(&u, &u_bar, kappa, iters, my_rank, num_procs);

	//all processes must be done before collecting results
	MPI_Barrier(MPI_COMM_WORLD);

	int total_datapoints = m*n;
	
	//The results of each process is gathered and stored in whole_image by process 0
	MPI_Gatherv(u.image_data, process_chunk_size, MPI_FLOAT, whole_image.image_data, &total_datapoints,process_row_skips, MPI_FLOAT, 0, MPI_COMM_WORLD);
	free(process_row_skips);

	//Process 0 must have obtained the entire image_chars array
	MPI_Barrier(MPI_COMM_WORLD);

	//process 0 creates and saves image, and deallocates whole_image
	if(my_rank==0) {
		convert_image_to_jpeg(&whole_image, image_chars);
		export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
		deallocate_image(&whole_image);
	}

	//Process 0 can not deallocate u as it has the same adress as whole_image
	if(my_rank!=0){deallocate_image(&u);}

	deallocate_image(&u_bar);
   
	MPI_Finalize ();
	return 0;
}
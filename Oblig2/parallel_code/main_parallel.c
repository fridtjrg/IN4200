#include "main_parallel.h"
#include <mpi.h>

void import_JPEG_file (const char* filename, unsigned char** image_chars,int* image_height,int* image_width,int* num_components);
void export_JPEG_file (const char* filename, const unsigned char* image_chars,int image_height, int image_width,int num_components, int quality);


/* declarations of functions import_JPEG_file and export_JPEG_file */
int main(int argc, char *argv[])
{
	int m, n, c, iters;
	int my_m, my_n, my_rank, num_procs;
	float kappa;
	image u, u_bar, whole_image;
	unsigned char *image_chars;
	char *input_jpeg_filename, *output_jpeg_filename;

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

	/* read from command line: kappa, iters, input_jpeg_filename, output_jpeg_file
	name */
	/* ... */

	if (my_rank==0) {
		import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);
		allocate_image (&whole_image, m, n);
	}

	MPI_Bcast (&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast (&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	/* 2D decomposition of the m x n pixels evenly among the MPI processes */
	int average_chunksize = floor(m/num_procs);
	//int chunksize_rest = m % num_procs;
	int process_boundary[2];

	process_boundary[0] = my_rank * average_chunksize;
	process_boundary[1] = (my_rank+1)* average_chunksize;

	//Last process implicitly will complete the "rest"
	if(my_rank == num_procs-1){
		process_boundary[1] = m;
	}

	my_m = process_boundary[1]- process_boundary[0];
	my_n = n;
	int process_chunk_size = my_m*my_n;


	//Contains the chunk size of each process
	int *process_chunk_sizes = malloc(num_procs*sizeof(*process_chunk_sizes));
	MPI_Gather(&process_chunk_size, 1, MPI_INT, &process_chunk_sizes[my_rank], 1, MPI_INT, 0, MPI_COMM_WORLD);


	printf("first gather done!\n");

	//Contains the index of each chunks first element in image_chars array
	int *chunk_idx = malloc(num_procs*sizeof(*chunk_idx));
    for(int proc = 0; proc < num_procs; proc++){
        chunk_idx[proc] = proc*process_chunk_sizes[proc];
    }

    unsigned char *my_image_chars = malloc(process_chunk_size*sizeof(my_image_chars));
	//MPI(Send data, How many to send of type, type, Recive databuffer, count, type, root, communicator)
	MPI_Scatterv(image_chars, process_chunk_sizes, chunk_idx, MPI_UNSIGNED_CHAR, my_image_chars, process_chunk_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
	printf("Scatter done!\n");


    allocate_image(&u, my_m, my_n);
    allocate_image(&u_bar, my_m, my_n);



	convert_jpeg_to_image(my_image_chars, &u);

	iso_diffusion_denoising_parallel(&u, &u_bar, kappa, iters, my_rank, num_procs);

	if(my_rank==0){
		printf("rank 0 u: \n");
		for(int i=0; i<process_chunk_size;i++){
			printf(" %f ",&&(u.image_data[i]));

		}
	}


	//all processes must be done before collecting results
	MPI_Barrier(MPI_COMM_WORLD);

	int *image_chunks = malloc(num_procs*sizeof(int));

	MPI_Gatherv(&(u.image_data), process_chunk_size, MPI_FLOAT, &(whole_image.image_data), image_chunks, chunk_idx, MPI_FLOAT, 0, MPI_COMM_WORLD);

	//Process one must have obtained the entire image_chars array
	MPI_Barrier(MPI_COMM_WORLD);



	if (my_rank==0) {
		convert_image_to_jpeg(&whole_image, image_chars);
		export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
		deallocate_image (&whole_image);
	}

	
	deallocate_image(&u);
	deallocate_image(&u_bar);
    free(image_chunks);
    free(my_image_chars);
	MPI_Finalize ();
	return 0;
}
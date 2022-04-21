#include "main_parallel.h"

/* declarations of functions import_JPEG_file and export_JPEG_file */
int main(int argc, char *argv[])
{
	int m, n, c, iters;
	int my_m, my_n, my_rank, num_procs;
	float kappa;
	image u, u_bar, whole_image;
	unsigned char *image_chars, *my_image_chars;
	char *input_jpeg_filename, *output_jpeg_filename;


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
	int average_chunksize = (*int)floor(m/num_procs);
	int chunksize_rest = m % num_procs;
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

	unsigned char *my_image_chars = malloc(process_chunk_size*sizeof(unsigned char));
	//MPI(Send data, How many to send of type, type, Recive databuffer, count, type, root, communicator)
	MPI_Scatterv(image_chars, process_chunk_size, MPI_UNSIGNED_CHAR, my_image_chars, process_chunk_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);



    allocate_image(&u, my_m, my_n);
    allocate_image(&u_bar, my_m, my_n);


	/* each process asks process 0 for a partitioned region */
	/* of image_chars and copy the values into u */
	/* ... */

	convert_jpeg_to_image (my_image_chars, &u);
	iso_diffusion_denoising_parallel(&u, &u_bar, kappa, iters, my_rank, num_procs);

	/* each process sends its resulting content of u_bar to process 0 */
	/* process 0 receives from each process incoming values and */
	/* copy them into the designated region of struct whole_image */
	/* ... */
	convert_image_to_jpeg(my_image_chars, &u);

	//all processes must be done before collecting results
	MPI_Barrier(MPI_COMM_WORLD);

	int *image_chunks = malloc(num_procs*sizeof(*int));

	MPI_Gatherv(my_image_chars, process_chunk_size, MPI_UNSIGNED_CHAR, image_chars, image_chunks, average_chunksize, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);



	if (my_rank==0) {
		export_JPEG_file(output_filename, image_chars, m, n, c, 75);
		//convert_image_to_jpeg(&whole_image, image_chars);
		//export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);
		//deallocate_image (&whole_image);
	}
	deallocate_image (&u);
	deallocate_image (&u_bar);
    free(image_chunks);
    free(my_image_chars);
	MPI_Finalize ();
	return 0;
}
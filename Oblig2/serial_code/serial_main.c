#include "serial_main.h"
#include <stdlib.h>
#include <stdio.h>



/* declarations of functions import_JPEG_file and export_JPEG_file */
void import_JPEG_file (const char* filename, unsigned char** image_chars,int* image_height,int* image_width,int* num_components);
void export_JPEG_file (const char* filename, const unsigned char* image_chars,int image_height, int image_width,int num_components, int quality);



int main(int argc, char *argv[])
{
	int m, n, iters;
	int c = 1;
	float kappa;
	image u, u_bar;
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
		iters= 10; //Many?
		input_jpeg_filename= "datafiles/mona_lisa_noisy.jpg";
		output_jpeg_filename = "datafiles/mona_lisa_serial_processed.jpg";
	}
	/* read from command line: kappa, iters, input_jpeg_filename, output_jpeg_filename */
	/* ... */



	import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);


	allocate_image(&u, m, n);
	allocate_image(&u_bar, m, n);

	convert_jpeg_to_image(image_chars, &u);


	iso_diffusion_denoising (&u, &u_bar, kappa, iters);

	//image_data[m][n]
	//gives same adress(troubleshooting)
	/*
	for(int i=0; i<m;i++){
		for(int j=0;j<n;j++){
			printf("First= %p, second=%p\n ",u.image_data[i]+j,&u.image_data[i][j]);
		}
	}

	printf("m= %d, n= %d",m,n); //m is larger
	*/

	convert_image_to_jpeg (&u_bar, image_chars);

	export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);

	deallocate_image (&u);
	deallocate_image (&u_bar);
	return 0;
}//0x55cc5b5bb5a8
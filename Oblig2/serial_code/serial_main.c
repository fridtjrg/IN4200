#include "serial_main.h"

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

		kappa = 0.1;
		iters= 100; 
		input_jpeg_filename= "datafiles/mona_lisa_noisy.jpg";
		output_jpeg_filename = "datafiles/mona_lisa_serial_processed.jpg";
	}

	//Importers jpeg file as 1d unsigned char array
	import_JPEG_file(input_jpeg_filename, &image_chars, &m, &n, &c);

	//Allocates memory for array in struct
	allocate_image(&u, m, n);
	allocate_image(&u_bar, m, n);

	//Converts the 1d unsigned char array to a 2d float array in struct
	convert_jpeg_to_image(image_chars, &u);

	//Performs ISO diffusion denoising
	iso_diffusion_denoising(&u, &u_bar, kappa, iters);

	//Converts the 2d float array from struct back into a 1d char array
	convert_image_to_jpeg (&u_bar, image_chars);

	//Saves file
	export_JPEG_file(output_jpeg_filename, image_chars, m, n, c, 75);

	//Deallocates
	deallocate_image(&u);
	deallocate_image(&u_bar);
	free(image_chars);
	return 0;
}//0x55cc5b5bb5a8
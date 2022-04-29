# Project 2

## Description
This folder contains all files relevant for project 2 in *IN4200*. Below you can find instructions for compiling and running the program,
as well as the content description. The prorgam is working as it should from what i can tell, and i get the same results as what has been provided.
The project can be found on github: https://github.com/fridtjrg/IN4200/edit/main/Oblig2/.

## Content
### simple-jpeg/
This folder contains the C-library for reading image files, provided from the project desciprion. 
For the main program to run, **import_export_jpeg.o** should be found in this folder.
If not, the makefile should create it automatically.

### datafiles/
This folder the original **mona_lisa_noisy.jpg** image. It is also the target folder for the main programs.
The processed images might already be there. If the program is run againg with different arguments,
these images will be overwritten.

### serial_code/
This folder contains the serial code. That is **serial_main.c**, **serial_main.h** and **functions.c**.
Note that the **functions.c** file for the parallel and serial code are almos identical,
but the one located in the parallell folder contains the parallel function for image denoising.

### parallel_code/
This folder contains the parallel code. That is **parallel_main.c**, **parallel_main.h** and **functions.c**.
Where **functions.c** is expanded upon from the serial version.

## Compilation
To compile both the serial and parallel program simply use: 
```
$ make
```
To only compile one of them use ```$ make serial``` or ```$ make parallel```. The makefile has all paths included,
so make sure you're located in the same folder as the makefilewhen using it.
You can also run the program with it's default values using ```$ make run-serial``` or ```$ make run-parallel```. 
Note that you can also use ```$ make clean``` to remove the compiled programs that end with ".exe".
Using clean will also remove all images stored in the **datafiles** folder that contain the
word "processed". 

The makefile should compile the library in simple-jpeg/ automatically.
If you do not see the __simple-jpeg/import_export_jpeg.o__ file after using the makefile, enter the folder
and use ```$make ``` there. 



## execution
Both programs take arguments in the following order:
```
main_serial.exe	kappa iters input_jpeg_filename	output_jpeg_filename
```
Where **kappa** is as small constant(typically below 0.2) and **iters** are the number of iterations done by the ISO diffusion denoising, both effect the resulting image. 
**input_jpeg_filename** is the name og the file to read and **output_jpeg_filename** is the name of the output file. One example of running can then be:
```
$ main_serial.exe 0.1 100 datafiles/mona_lisa_noisy.jpg datafiles/mona_lisa_serial_processed.jpg
```
or
```
$ main_parallel.exe 0.1 1000 datafiles/mona_lisa_noisy.jpg datafiles/mona_lisa_parallel_processed.jpg
```

Note that if the program does not recive the correct number of arguments, it will run with it's default values which are:<br/>

For parallel: kappa = 0.1, iters= 1000, input_jpeg_filename= "datafiles/mona_lisa_noisy.jpg", output_jpeg_filename = "datafiles/mona_lisa_parallel_processed.jpg".
<br/>

For serial: kappa = 0.1, iters= 100, input_jpeg_filename= "datafiles/mona_lisa_noisy.jpg", output_jpeg_filename = "datafiles/mona_lisa_serial_processed.jpg".


# Project 1

## Description
This folder contains all files relevant for project 1 in *IN4200*. Below you can find instructions for compiling and running the program, as well as the content description. The prorgam is working as it should from what i can tell, and i get the same results as what has been provided. The project can be found on github: https://github.com/fridtjrg/IN4200/tree/main/oblig1.

## Content
### src/
This folder contains the source files neede to compile the main program, as well as the headerfiles.

### datafiles/
This folder contains the datafiles read by the program when executed.

## Compilation
To compile the program using the makefile simply use: 
```
$ make
```
this will create __*main.exe*__ which can be executed with the appropriate arguments. If you wish to compile the program manually, you can also use: 
```
$ gcc -o main.exe main.c src/read_graph_from_file.c src/PageRank_iterations.c src/top_n_webpages.c -fopenmp
```
Note that you can use:
```
make clean
```
To remove the executable file.

## Executing
After the program has been compiled, you can execute the file. The arguments are taken in the following order:
```
main.exe filename d epsilon n
```
Where **filename** is the name of the file you want to read(*If it is located in a subdirectory, this must be specified*), **d** is the dampening constant, **epsilon** is the convergence threshold value and **n** is the top n webpages you want to be printed. An example of running the program is then:

```
$ ./main.exe datafiles/100nodes.txt 0.85 0.000001 4
```
Where filename=*datafiles/100nodes.txt*, d=*0.85*, epsilon=*0.000001* and n=*4*. If the function does not recive the appropriate number of input arguments, it will run with the default values which are: 	file_name = "datafiles/100nodes.txt", d=0.85, epsilon= 10e-7, n = 3.


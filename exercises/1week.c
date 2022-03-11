#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include "1week_head.h"

void task1();
void task2();
void array3d();
void readfromfile();


int main()
{
  //task1();
  //task2();
  task3();



  return 0;
}

void task2(){

  //declaring variables
  int n =10, i, *ptr, sum = 0;

  //allocating memory
  ptr = (int*) malloc(n * sizeof(int));
  if(ptr == NULL) {
    printf("Error! memory not allocated."); exit(-1);
  }

  //Filling in random elements
  for (i = 0; i < n; ++i) {
    ptr[i] = rand();
  }

  //printing elemnts in array
  for (i = 0; i < n; ++i) {
      printf("%d\n", ptr[i]);
    }

  //frees memory.
  free(ptr);
}

void task1(){
  double limit, tolerance, my_sum, add;
  int max_iter;

  limit = 4./5.;
  tolerance = pow(10,-12);


  my_sum=0;
  add = 1;

  for(int n=0;n<=30;n++){
    my_sum += add;
    add = -add/(2*2);
  }

  printf("With %d iterations the sum gave %f.\n", 30, my_sum);
  printf("The error is %f \n", limit-my_sum);
}

void array3d(){

    //3d array
    int Nx, Ny, Nz;

    int ***A;

    Nx = Ny = Nz = 3;

    A = (int ***)malloc(Nz * sizeof(int **));
    A[0] = (int **)malloc(Nz * Ny * sizeof(int *));
    A[0][0] = (int *)malloc(Nz * Ny * Nx * sizeof(int));

    // First layer of pointers.
    // The 3d array contains Nz "slices," each of which has Ny columns.
    for (int i = 1; i < Nz; i ++) {
        A[i] = &A[0][Ny * i];
    }

    // Second layer of pointers.
    // The 3d array contains Ny * Nz rows, each of which has Nx cells.
    for (int j = 1; j < Nz * Ny; j ++) {
        A[0][j] = &A[0][0][j * Nx];
    }

    // Assign some values.
    for (int i = 0; i < Nz; i ++) {
        for (int j = 0; j < Ny; j ++) {
            for (int k = 0; k < Nx; k ++) {
                A[i][j][k] = Nx * Ny * i + Ny * j + k;
                printf("(%d %d %d): %d ", i, j, k, A[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }

    // Last step
    free(A[0][0]);
    free(A[0]);
    free(A);
}

int readfromfile(int narg, char **argv) {

    temperature_data *data;
    int minidx, maxidx;
    double avg, dev;

    if (narg < 2) {

        printf("Filename required.\n");
        exit(0);
    }

    data = (temperature_data *)malloc(sizeof(temperature_data));

    readfile(argv[1], data);

    minidx = findminidx(data->temps, data->n);
    maxidx = findmaxidx(data->temps, data->n);
    avg = average(data->temps, data->n);
    dev = stdev(data->temps, data->n, avg);

    for (int i = 0; i < data->n; i ++) {
        printf("%.5s %lf\n", &data->times[5 * i], data->temps[i]);
    }

    printf("Lowest temperature  %f at time %.5s\n", data->temps[minidx], &data->times[5 * minidx]);
    printf("Highest temperature  %f at time %.5s\n", data->temps[maxidx], &data->times[5 * maxidx]);
    printf("Mean temperature %f, with standard deviation %f.\n", avg, dev);

    free(data->times);
    free(data->temps);
    free(data);
}

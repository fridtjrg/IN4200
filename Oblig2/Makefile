CC = gcc
PCC = mpicc
CFLAGS = -O2
LDFLAGS = -L./simple-jpeg/ -lsimplejpeg

PROJ = serial_main
OBJS = serial_main.o

all: main_parallel.exe
	./main_parallel.exe

serial: main_serial.exe
	./main_serial.exe

parallel: main_parallel.exe
	./main_parallel.exe


main_serial.exe:  serial_code/serial_main.c serial_code/functions.c 
	cd simple-jpeg && make
	${CC} -o $@  $^ ${LDFLAGS} -Wall -g

main_parallel.exe:  parallel_code/main_parallel.c parallel_code/functions.c 
	cd simple-jpeg && make
	${PCC} -o $@  $^ ${LDFLAGS} -Wall -g

clean:
	rm -f *.exe *~ 
	cd datafiles && rm -f *processed*





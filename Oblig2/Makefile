CC = gcc
CFLAGS = -O2
LDFLAGS = -L./simple-jpeg/ -lsimplejpeg

PROJ = serial_main
OBJS = serial_main.o

all: main_parallel.exe
	./main_parallel.exe

serial: main_serial.exe
	./main_serial.exe


main_serial.exe:  serial_code/serial_main.c serial_code/functions.c 
	#${CPPflags} -o main.exe main.c src/read_graph_from_file.c -Wall
	${CC} -o $@  $^ ${LDFLAGS} -Wall -g

clean:
	rm -f *.exe *~





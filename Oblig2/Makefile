
Cflags = gcc  # -wall

all: main_serial.exe
	./main_serial.exe


main_serial.exe:  serial_code/serial_main.c src/simple-jpeg/import_export_jpeg.c
	#${CPPflags} -o main.exe main.c src/read_graph_from_file.c -Wall
	${Cflags} -o $@ $^ -fopenmp -Wall -g

clean:
	rm -f *.exe *~





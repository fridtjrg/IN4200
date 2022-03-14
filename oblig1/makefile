
Cflags = gcc  # -wall

all: main.exe
	./main.exe


main.exe:  main.c src/read_graph_from_file.c src/PageRank_iterations.c
	#${CPPflags} -o main.exe main.c src/read_graph_from_file.c -Wall
	${Cflags} -o $@ $^ -Wall

clean:
	rm -f *.exe *~

plot:




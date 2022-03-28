
Cflags = gcc  # -wall

all: main.exe

run: main.exe
	./main.exe

main.exe:  main.c src/read_graph_from_file.c src/PageRank_iterations.c src/top_n_webpages.c 
	#${CPPflags} -o main.exe main.c src/read_graph_from_file.c -Wall
	${Cflags} -o $@ $^ -fopenmp -Wall -g

clean:
	rm -f *.exe *~





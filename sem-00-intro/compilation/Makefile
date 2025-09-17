all: main.c solution.c solution.h
	gcc main.c solution.c

main.i: main.c solution.h
	gcc -E main.c -o main.i
solution.i: solution.c solution.h
	gcc -E solution.c -o solution.i

main.s: main.i
	gcc -S main.i
solution.s: solution.i
	gcc -S solution.i

main.o: main.s
	gcc -c main.s
solution.o: solution.s
	gcc -c solution.s

main: main.o solution.o
	gcc main.o solution.o -o main

clean:
	rm a.out *.o *.s *.i main

all: main.c solution.c solution.h
	gcc main.c solution.c

main.o: main.c solution.h
	gcc -c main.c
solution.o: solution.c solution.h
	gcc -c solution.c

main.s: main.c solution.h
	gcc -S main.c
solution.s: solution.c solution.h
	gcc -S solution.c

main: main.o solution.o
	gcc main.o solution.o -o main
main_asm: main.s solution.s
	gcc main.s solution.s -o main_asm

clean:
	rm a.out *.o *.s main main_asm

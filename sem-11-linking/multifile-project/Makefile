run: a.out
	./a.out; echo return code: $$?

study.s: ../libcaos/study.c
	cc -O2 -masm=intel -S ../libcaos/study.c -o study.s

study.o: study.s
	cc -O2 -c study.s -o study.o

grade.o: ../libcaos/grade.c
	cc -c ../libcaos/grade.c -o grade.o

main.s: main.c
	cc -O2 -masm=intel -S main.c

main.o: main.s
	cc -O2 -c main.s

a.out: study.o grade.o main.o
	cc study.o grade.o main.o 

clean:
	rm a.out *.s *.o

run: a.out
	./a.out; echo $$?

study.s: ../libcaos/study.c
	cc -O2 -masm=intel -S ../libcaos/study.c -o study.s

study.o: study.s
	cc -O2 -c study.s -o study.o

grade.o: ../libcaos/grade.c
	cc -c ../libcaos/grade.c -o grade.o

lib/libcaos.a: grade.o study.o
	ar rcs lib/libcaos.a grade.o study.o

main.s: main.c
	cc -I../libcaos -O2 -masm=intel -S main.c

main.o: main.s
	cc -I../libcaos -O2 -c main.s

a.out: main.o lib/libcaos.a
	cc -Llib main.o -lcaos 

clean:
	rm a.out *.s *.o lib/*.a

run: a.out
	LD_LIBRARY_PATH=lib ./a.out; echo return code: $$?

study.s: ../libcaos/study.c
	cc $(opts) -fPIC -O2 -masm=intel -S ../libcaos/study.c -o study.s

study.o: study.s
	cc $(opts) -fPIC -O2 -c study.s -o study.o

grade.o: ../libcaos/grade.c
	cc $(opts) -c ../libcaos/grade.c -o grade.o

lib/libcaos.so: grade.o study.o
	cc $(opts) -shared -fPIC study.o grade.o -o lib/libcaos.so

main.s: main.c
	cc $(opts) -O2 -I../libcaos -masm=intel -S main.c

main.o: main.s
	cc $(opts) -O2 -I../libcaos -c main.s

a.out: main.o lib/libcaos.so
	cc $(opts) -Llib -Wl,-z,lazy main.o -lcaos

clean:
	rm a.out *.s *.o lib/*.so

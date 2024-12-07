run: a.out
	./a.out; echo $$?

libcaos/foo.o: libcaos/foo.c
	cc -c libcaos/foo.c -o libcaos/foo.o

libcaos/x.o: libcaos/x.c
	cc -c libcaos/x.c -o libcaos/x.o

libcaos/libcaos.a: libcaos/x.o libcaos/foo.o
	ar rcs libcaos/libcaos.a libcaos/x.o libcaos/foo.o

main.o: main.c
	cc -I. -c main.c

a.out: main.o libcaos/libcaos.a
	cc main.o -lcaos -Llibcaos

clean:
	rm a.out *.o libcaos/*.o libcaos/*.a

run: a.out
	./a.out; echo $$?

libcaos/foo.o: libcaos/foo.c
	cc -g -c libcaos/foo.c -o libcaos/foo.o

libcaos/x.o: libcaos/x.c
	cc -g -c libcaos/x.c -o libcaos/x.o

libcaos/libcaos.so: libcaos/x.o libcaos/foo.o
	cc -g -shared -fPIC libcaos/foo.o libcaos/x.o -o libcaos/libcaos.so

main.o: main.c
	cc -g -I. -c main.c

a.out: main.o libcaos/libcaos.so
	cc -g -Llibcaos -Wl,-z,lazy -Wl,-rpath=libcaos main.o -lcaos

clean:
	rm a.out *.o libcaos/*.o libcaos/*.so

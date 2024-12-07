run: a.out
#	LD_LIBRARY_PATH=libcaos 
	./a.out; echo $$?

libcaos/foo.o: libcaos/foo.c
	cc -c libcaos/foo.c -o libcaos/foo.o

libcaos/x.o: libcaos/x.c
	cc -c libcaos/x.c -o libcaos/x.o

libcaos/libcaos.so: libcaos/x.o libcaos/foo.o
	cc -shared -fPIC libcaos/foo.o libcaos/x.o -o libcaos/libcaos.so

main.o: main.c
	cc -I. -fno-plt -c main.c

a.out: main.o libcaos/libcaos.so
	cc -Llibcaos main.o -lcaos -Wl,-rpath=libcaos

clean:
	rm a.out *.o libcaos/*.o libcaos/*.so

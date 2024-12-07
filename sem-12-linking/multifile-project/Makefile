run: a.out
	./a.out; echo $$?

foo.o: foo.c
	cc -g -c foo.c

x.o: x.c
	cc -g -c x.c

main.o: main.c
	cc -g -c -O2 main.c

a.out: foo.o x.o main.o
	cc -g foo.o x.o main.o 

clean:
	rm a.out *.o

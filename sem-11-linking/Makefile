stdlib.o: stdlib.c
	cc -O2 -c stdlib.c 

static: stdlib.o
	cc -static stdlib.o

no-pie: stdlib.o
	cc -no-pie stdlib.o

dynamic: stdlib.o
	cc -O2 stdlib.o

clean:
	rm a.out *.o

.PHONY: clean static dynamic no-pie

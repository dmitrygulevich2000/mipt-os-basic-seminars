stdlib.o: stdlib.c
	cc -g -c stdlib.c 

static: stdlib.o
	cc -g -static stdlib.o

no-pie: stdlib.o
	cc -g -no-pie stdlib.o

dynamic: stdlib.o
	cc -g stdlib.o

clean:
	rm a.out *.o

.PHONY: clean static dynamic no-pie

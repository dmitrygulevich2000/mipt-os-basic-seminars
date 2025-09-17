io:
	gcc io.c --output io && ./io

list:
	gcc list.c -o list && ./list

list_gdb:
	gcc -g list.c && gdb ./a.out

list_asan:
	gcc -g -fsanitize=address list.c && ./a.out

vec:
	gcc vec.c && ./a.out

vec_asan:
	gcc -g -fsanitize=address vec.c && ./a.out

clean:
	rm io list a.out

.PHONY: io list
generate:
	python3 generate_array.py 80000 2.5 1 > data.txt

main:
	cc -mavx sse.s avx.s main.c

run: main
	./a.out $(algo) <data.txt

clean:
	rm -f a.out data.txt


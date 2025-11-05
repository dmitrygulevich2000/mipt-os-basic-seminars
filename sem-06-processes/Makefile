fork:
	cc fork.c && ./a.out
map_shared:
	cc -fsanitize=leak map_shared.c && ./a.out
exec:
	cc exec.c
	echo "a.out can now be used instead of mkdir"

shell:
	cc shell.c && ./a.out
redirecting_shell:
	cc redirecting_shell.c && ./a.out

threads:
	cc -pthread threads.c && ./a.out

prepare_array:
	../sem-04-simd-arm/simd/generate_array.py 8000000 2.5 1 >array.txt
array_avg: 
	cc -pthread array_avg.c && ./a.out <array.txt
	echo "or another thread_num can be passed to a.out"

clean:
	rm a.out array.txt
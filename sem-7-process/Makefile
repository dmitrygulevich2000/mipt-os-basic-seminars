fork:
	gcc fork.c && ./a.out
map_shared:
	gcc -fsanitize=leak map_shared.c && ./a.out
exec:
	gcc exec.c
	echo "a.out can now be used instead of mkdir"

shell:
	gcc shell.c && ./a.out
redirecting_shell:
	gcc redirecting_shell.c && ./a.out

threads:
	gcc -pthread threads.c && ./a.out

prepare_array:
	../sem-5-simd-arm/simd/generate_array.py 8000000 2.5 1 >array.txt
array_avg:
	gcc -pthread array_avg.c && ./a.out <array.txt
	echo "or another thread_num can be passed to a.out"

clean:
	rm a.out array.txt
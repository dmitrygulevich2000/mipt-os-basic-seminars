stack_address:
	cc stack_address.c && ./a.out

global_address:
	cc -no-pie global_address.c && ./a.out

array_sum:
	cc array_sum.c && ./a.out

page_fault:
	cc page_fault.c && sudo perf stat ./a.out

malloc:
	cc malloc.c && strace ./a.out

cpu_cache:
	cc -O1 cpu_cache.c && ./a.out 10000000

cpu_cache_stat:
	cc cpu_cache.c && sudo perf stat -e cache-references,cache-misses ./a.out 10000000

mat_mul:
	cc mat_mul.c && ./a.out 1000

mat_mul_stat:
	cc mat_mul.c && sudo perf stat -e cache-references,cache-misses ./a.out 1000

clean:
	rm a.out

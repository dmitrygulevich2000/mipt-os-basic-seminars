run: seccomp
	./a.out $(cmd)

run_hacked: seccomp write.so
	LD_PRELOAD=./write.so ./a.out $(cmd)

seccomp: seccomp.c
	cc seccomp.c

write.so: write.c
	cc -fPIC -shared write.c -ldl -o write.so

clean:
	rm a.out *.so

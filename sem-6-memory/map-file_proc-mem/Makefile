map_file:
	cat coliseum.txt
	gcc map_file.c && ./a.out coliseum.txt
	cat coliseum.txt

mem:
	gcc mem.c -o mem.out && \
	sudo setcap cap_sys_ptrace+ep mem.out && \
	echo "usage: ./mem.out pid address"

clean:
	rm a.out mem.out

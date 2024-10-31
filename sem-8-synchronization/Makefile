ifeq ($(tsan),)
else
	opts = -fsanitize=thread
endif

race_condition:
	cc $(opts) -pthread race_condition.c && ./a.out
data_race:
	cc $(opts) -pthread data_race.c && ./a.out
store_buffering:
	cc $(opts) -O2 -pthread store_buffering.c && ./a.out

mutex:
	cc $(opts) -pthread mutex.c && ./a.out
atomic:
	cc $(opts) -pthread -latomic atomic.c && ./a.out
store_buffering_bye:
	cc $(opts) -O2 -pthread -latomic store_buffering_bye.c && ./a.out

condvar:
	cc $(opts) -pthread condvar.c && ./a.out
lock-free:
	cc $(opts) -O2 -pthread -latomic lock-free.c && ./a.out

clean:
	rm a.out

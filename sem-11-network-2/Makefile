ifeq ($(release),)
else
	opts = -DRELEASE -O2
endif

threading_server:
	cc $(opts) -pthread threading_server.c

epoll_server:
	cc epoll_server.c

epoll_server_stateful:
	cc $(opts) epoll_server_stateful.c

epoll_server_fast:
	cc $(opts) epoll_server_fast.c

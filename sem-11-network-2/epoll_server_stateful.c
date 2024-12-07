#include <arpa/inet.h>
#include <assert.h>
#include <err.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

char addr_str[256];

char* straddr(const struct sockaddr_in* client_addr) {
    assert(client_addr->sin_family == AF_INET);
    char* addr_str = inet_ntoa(client_addr->sin_addr);
    uint16_t port = ntohs(client_addr->sin_port);

    sprintf(addr_str, "%s %hu", addr_str, port);
    return addr_str;
}

///////////////////////////////////////////////////////////////

typedef struct {
    int socket;
    char buf[4096];
    size_t buf_begin;
    size_t buf_end;
} client_state_t;

void close_connection(int epollfd, client_state_t* client) {
    shutdown(client->socket, SHUT_RDWR);
    close(client->socket);  // also deleted from epoll*
    free(client);
}

void handle_client_event(int epollfd, const struct epoll_event* event) {
    client_state_t* client = event->data.ptr;
    
    // hang up
    if (event->events & EPOLLHUP) {
        close_connection(epollfd, client);
        return;
    }
    
    // data available and buffer is empty
    if ((event->events & EPOLLIN) && client->buf_end == 0) {
        ssize_t rbytes = read(client->socket, client->buf, sizeof(client->buf));
        if (rbytes == 0) {
            close_connection(epollfd, client);
            return;
        }
        if (rbytes < 0) {
            err(1, "read from socket %d", client->socket);
        }
        client->buf_end += rbytes;
        return;
    }

    // may write and buffer is non-empty
    if ((event->events & EPOLLOUT) && client->buf_end > 0) {
        ssize_t wbytes = write(
            client->socket, 
            client->buf + client->buf_begin, 
            client->buf_end - client->buf_begin
        );
        if (wbytes < 0) {
            if (errno == EPIPE) {
                close_connection(epollfd, client);
                return;
            }
            err(1, "write to socket %d", client->socket);
        }
        client->buf_begin += wbytes;
        if (client->buf_begin == client->buf_end) {
            client->buf_begin = 0;
            client->buf_end = 0;
        }
        return;
    }
}

void handle_conn_event(int epollfd, const struct epoll_event* event) {
    int server_socket = (int)(uint64_t)event->data.ptr;

    // accept connection
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
    if (client_socket < 0) {
        err(1, "accept");
    }
#ifndef RELEASE
    fprintf(stderr, "Connection received on %s\n", straddr(&client_addr));
#endif

    // register client event
    client_state_t* client = malloc(sizeof(client_state_t));
    client->socket = client_socket;
    client->buf_begin = 0;
    client->buf_end = 0;

    struct epoll_event client_event;
    client_event.events = EPOLLHUP | EPOLLIN | EPOLLOUT;
    client_event.data.ptr = client;
    
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, client_socket, &client_event) < 0) {
        err(1, "epoll_ctl add client event");
    }
}

void run(int epollfd, int server_socket) {
    // register connection event
    struct epoll_event conn_event;
    conn_event.events = EPOLLIN;
    conn_event.data.ptr = (void*)(uint64_t)server_socket;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server_socket, &conn_event) < 0) {
        err(1, "epoll_ctl add connection event");
    }

    // process events
    const int MAX_EPOLL_EVENTS = 64000;
    struct epoll_event events[MAX_EPOLL_EVENTS];
    while (1) {
        int event_count = epoll_wait(epollfd, events, MAX_EPOLL_EVENTS, -1);
        if (event_count < 0) {
            err(1, "epoll_wait");
        }

        for (int i = 0; i < event_count; ++i) {
            struct epoll_event* event = &events[i];
            if ((int)(uint64_t)event->data.ptr == server_socket) {
                handle_conn_event(epollfd, event);
            } else {
                handle_client_event(epollfd, event);
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////

const char* BIND_ADDRESS = "0.0.0.0";

int prepare_server_socket(uint16_t hport) {
    // prepare address to listen on
    struct in_addr ip_addr;
    if (inet_aton(BIND_ADDRESS, &ip_addr) == 0) {
        errx(1, "invalid address %s", BIND_ADDRESS);
    }

    struct sockaddr_in listen_addr;
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(hport);
    listen_addr.sin_addr = ip_addr;

    // create & setup socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        err(1, "socket");
    }
    if (bind(server_socket, (struct sockaddr*)&listen_addr, sizeof(listen_addr)) < 0) {
        err(1, "bind");
    }
    if (listen(server_socket, 256) < 0) {
        err(1, "listen");
    }

    return server_socket;
}

/////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    signal(SIGPIPE, SIG_IGN);

    // parse arguments
    if (argc != 2) {
        errx(1, "usage: %s port", argv[0]);
    }
    char* err_pos;
    uint16_t port = strtoul(argv[1], &err_pos, 10);
    if (*err_pos != '\0') {
        errx(1, "invalid port");
    }

    // do server business
    int epollfd = epoll_create(1);
    if (epollfd < 0) {
        err(1, "epoll_create");
    }
    int server_socket = prepare_server_socket(port);

    fprintf(stderr, "Listening on %s %hu\n", BIND_ADDRESS, port);

    run(epollfd, server_socket);
}

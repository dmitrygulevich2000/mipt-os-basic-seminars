#include <arpa/inet.h>
#include <assert.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
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

int make_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL);
    if (flags < 0) {
        return -1;
    }
    flags |= O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags);
}

///////////////////////////////////////////////////////////////

typedef struct {
    int socket;
    char buf[4096];
    size_t buf_begin;
    size_t buf_end;
} socket_state_t;

ssize_t client_read(socket_state_t* client) {
    assert(client->buf_begin == 0);
    ssize_t rbytes = read(client->socket, client->buf, sizeof(client->buf));
    if (rbytes <= 0) {
        return rbytes;
    }
    client->buf_end += rbytes;
    return rbytes;
}

ssize_t client_write(socket_state_t* client) {
    ssize_t wbytes = write(
        client->socket, 
        client->buf + client->buf_begin, 
        client->buf_end - client->buf_begin
    );
    if (wbytes < 0) {
        return wbytes;
    }
    client->buf_begin += wbytes;
    if (client->buf_end == client->buf_begin) {
        client->buf_begin = 0;
        client->buf_end = 0;
    }
    return wbytes;
}

void close_connection(int epollfd, socket_state_t* client) {
    shutdown(client->socket, SHUT_RDWR);
    close(client->socket);  // also deleted from epoll*
    free(client);
}

int handle_client_until_eagain(int epollfd, struct epoll_event* trigger_event) {
    socket_state_t* client = trigger_event->data.ptr;

    while (client->buf_end != 0) {
        ssize_t wbytes = client_write(client);
        if (wbytes < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // go wait for events
                return 0;
            }
            return -2;
        }
    }

    ssize_t rbytes = 0;
    while ((rbytes = client_read(client)) > 0) {
        while (client->buf_end != 0) {
            ssize_t wbytes = client_write(client);
            if (wbytes < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // go wait for events
                    return 0;
                }
                return -2;
            }
        }
    }
    if (rbytes < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // go wait for events
            return 0;
        }
        return -1;
    }
    // rbytes == 0
    close_connection(epollfd, client);
    return 0;
}

void handle_client_event(int epollfd, struct epoll_event* event) {
    if (event->events & (EPOLLHUP | EPOLLERR)) {
        fprintf(stderr, "got epollhup or epollerr\n");
        close_connection(epollfd, event->data.ptr);
        return;
    }
    int status = handle_client_until_eagain(epollfd, event);
    if (status < 0) {
        fprintf(stderr, "communication failed with error %d, cause: %s\n", -status, strerror(errno));
        close_connection(epollfd, event->data.ptr);
    }
}

void accept_connections_until_eagain(
    int epollfd, 
    const struct epoll_event* trigger_event
) {
    int server_socket = ((socket_state_t*)trigger_event->data.ptr)->socket;
    
    while (1) {
        // try accept connection
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        if (client_socket < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // go wait for events
                return;
            }
            err(1, "accept");
        }
        // make client socket nonblocking
        if (make_nonblocking(client_socket) < 0) {
            err(1, "make_nonblocking client");
        }
#ifndef RELEASE
        fprintf(stderr, "Connection received on %s\n", straddr(&client_addr));
#endif

        // register client event
        socket_state_t* client = malloc(sizeof(socket_state_t));
        client->socket = client_socket;
        client->buf_begin = 0;
        client->buf_end = 0;

        struct epoll_event client_event;
        client_event.events = EPOLLHUP | EPOLLIN | EPOLLOUT | EPOLLET;
        client_event.data.ptr = client;
        
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, client_socket, &client_event) < 0) {
            err(1, "epoll_ctl add client event");
        }

        // try handle client
        int status = handle_client_until_eagain(epollfd, &client_event);
        if (status < 0) {
            fprintf(stderr, "communication failed with error %d, cause: %s\n", -status, strerror(errno));
            close_connection(epollfd, client);
        }
    }
}

void handle_conn_event(int epollfd, const struct epoll_event* event) {
    accept_connections_until_eagain(epollfd, event);
}

void run(int epollfd, int server_socket) {
    // register connection event
    socket_state_t* server = malloc(sizeof(socket_state_t));
    server->socket = server_socket;

    struct epoll_event conn_event;
    conn_event.events = EPOLLIN | EPOLLET;
    conn_event.data.ptr = server;
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
            if (((socket_state_t*)event->data.ptr)->socket == server_socket) {
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
    if (make_nonblocking(server_socket) < 0) {
        err(1, "make_nonblocking server");
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

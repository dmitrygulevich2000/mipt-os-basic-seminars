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

void close_connection(int epollfd, int client_socket) {
    shutdown(client_socket, SHUT_RDWR);
    close(client_socket);  // also deleted from epoll*
}

void handle_client_event(int epollfd, const struct epoll_event* event) {
    int client_socket = event->data.fd;

    // close if hang up
    if (event->events & EPOLLHUP) {
        close_connection(epollfd, client_socket);
        return;
    }

    // read data
    assert(event->events & EPOLLIN);
    char buffer[4096];
    ssize_t rbytes = read(client_socket, buffer, sizeof(buffer));
    if (rbytes == 0) {
        close_connection(epollfd, client_socket);
        return;
    }
    assert(rbytes > 0);

    // write back
    ssize_t wbytes = write(client_socket, buffer, rbytes);
    assert(wbytes == rbytes);
}

void handle_conn_event(int epollfd, const struct epoll_event* event) {
    int server_socket = event->data.fd;

    // accept connection
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
    if (client_socket < 0) {
        err(1, "accept");
    }
    fprintf(stderr, "Connection received on %s\n", straddr(&client_addr));

    // register event triggered by incoming data
    struct epoll_event client_event;
    client_event.events = EPOLLIN | EPOLLHUP;
    client_event.data.fd = client_socket;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, client_socket, &client_event) < 0) {
        err(1, "epoll_ctl add client event");
    }
}

void run(int epollfd, int server_socket) {
    // register event triggered by incoming connection
    struct epoll_event conn_event;
    conn_event.events = EPOLLIN;
    conn_event.data.fd = server_socket;
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
            if (event->data.fd == server_socket) {
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

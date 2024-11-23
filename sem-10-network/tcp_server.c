#include <arpa/inet.h>
#include <assert.h>
#include <err.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int handle_client(int client_socket) {
    char buffer[4096];
    ssize_t rbytes = 0;
    while ((rbytes = read(client_socket, buffer, sizeof(buffer))) > 0) {
        ssize_t wbytes = write(client_socket, buffer, rbytes);
        if (wbytes < 0) {
            return -2;
        }
        assert(wbytes == rbytes);
    }
    if (rbytes < 0) {
        return -1;
    }
    if (shutdown(client_socket, SHUT_RDWR) < 0) {
        return -3;
    }
    return 0;
}

void run(int server_socket) {
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        if (client_socket < 0) {
            err(1, "accept");
        }
        fprintf(stderr, "Connection received on %s\n", straddr(&client_addr));

        int status = handle_client(client_socket);
        if (status < 0) {
            fprintf(stderr, "communication failed with error %d, cause: %s\n", -status, strerror(errno));
        }
        close(client_socket);
    }
}

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
    int server_socket = prepare_server_socket(port);
    fprintf(stderr, "Listening on %s %hu\n", BIND_ADDRESS, port);  
    // or get port from getsockname() if 0 passed

    run(server_socket);
}

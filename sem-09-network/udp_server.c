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

void run_udp_server(int server_socket) {
    char buffer[4096];
    struct sockaddr_in sender_addr;
    socklen_t addr_len = sizeof(sender_addr);

    while (1) {
        // receive datagram
        ssize_t rbytes = recvfrom(
            server_socket, buffer, sizeof(buffer), 0,
            (struct sockaddr*)&sender_addr, &addr_len
        );
        if (rbytes < 0) {
            fprintf(stderr, "recvfrom error: %s\n", strerror(errno));
            continue;
        }
        fprintf(stderr, "Datagram received from %s\n", straddr(&sender_addr));

        // send datagram back
        ssize_t wbytes = sendto(
            server_socket, buffer, rbytes, 0,
            (struct sockaddr*)&sender_addr, addr_len
        );
        if (wbytes < 0) {
            fprintf(stderr, "sendto error: %s", strerror(errno));
            continue;
        }
        assert(wbytes == rbytes);
    }
}

const char* BIND_ADDRESS = "0.0.0.0";

int prepare_udp_server_socket(uint16_t hport) {
    // prepare address to bind to
    struct in_addr ip_addr;
    if (inet_aton(BIND_ADDRESS, &ip_addr) == 0) {
        errx(1, "invalid address %s", BIND_ADDRESS);
    }

    struct sockaddr_in bind_addr;
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_port = htons(hport);
    bind_addr.sin_addr = ip_addr;

    // create & setup socket
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        err(1, "socket");
    }
    if (bind(server_socket, (struct sockaddr*)&bind_addr, sizeof(bind_addr)) < 0) {
        err(1, "bind");
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
    int server_socket = prepare_udp_server_socket(port);
    fprintf(stderr, "Bound on %s %hu\n", BIND_ADDRESS, port);

    run_udp_server(server_socket);
}

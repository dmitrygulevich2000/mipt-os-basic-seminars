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

int connect_to_server(const struct in_addr* ip_addr, uint16_t hport) {
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(hport);
    server_addr.sin_addr = *ip_addr;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        err(1, "socket");
    }
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        err(1, "connect");
    }
    return sockfd;
}

void handle_input(int sockfd) {
    char buffer[4096];
    ssize_t rbytes = 0;
    while ((rbytes = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
        ssize_t wbytes = write(sockfd, buffer, rbytes);
        if (wbytes < 0) {
            err(1, "write to server");
        }
        assert(wbytes == rbytes);
    }
    if (rbytes < 0) {
        err(1, "read from input");
    }
    if (shutdown(sockfd, SHUT_RDWR) < 0) {
        err(1, "shutdown");
    }
}

int main(int argc, char** argv) {
    signal(SIGPIPE, SIG_IGN);

    // parse arguments
    if (argc < 3) {
        errx(1, "usage: %s ip-address port", argv[0]);
    }
    struct in_addr addr;
    if (inet_aton(argv[1], &addr) == 0) {
        errx(1, "invalid address");
    }
    char* err_pos;
    uint16_t port = strtoul(argv[2], &err_pos, 10);
    if (*err_pos != '\0') {
        errx(1, "invalid port");
    }

    // do tcp client business
    int sockfd = connect_to_server(&addr, port);
    fprintf(stderr, "connected to the server\n");

    handle_input(sockfd);
}

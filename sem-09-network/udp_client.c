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

void handle_input(int sockfd, const struct sockaddr_in* server_addr) {

    char buffer[4096];
    ssize_t rbytes = 0;
    while ((rbytes = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
        ssize_t wbytes = sendto(
            sockfd, buffer, rbytes, 0,
            (struct sockaddr*)server_addr, sizeof(struct sockaddr_in)
        );
        if (wbytes < 0) {
            err(1, "write to server");
        }
        assert(wbytes == rbytes);
    }
    if (rbytes < 0) {
        err(1, "read from input");
    }
}

int main(int argc, char** argv) {
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

    // prepare server sockaddr
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = addr;
    // create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        err(1, "socket");
    }
    fprintf(stderr, "ready to send datagrams\n");

    handle_input(sockfd, &server_addr);
}

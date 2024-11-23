#include <arpa/inet.h>
#include <assert.h>
#include <err.h>
#include <inttypes.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        errx(1, "usage: %s hostname", argv[0]);
    }

    struct addrinfo* result;
    getaddrinfo(argv[1], "80", NULL, &result);

    struct addrinfo* curr = result;
    for (; curr != NULL; curr = curr->ai_next) {
        if (curr->ai_family == AF_INET) {
            struct sockaddr_in* ip_addr = (struct sockaddr_in*)curr->ai_addr;
            char* addr_str = inet_ntoa(ip_addr->sin_addr);
            uint16_t port = ntohs(ip_addr->sin_port);

            printf(
                "found address: (socktype: %2d, protocol: %2d) %s %hu\n", 
                curr->ai_socktype, curr->ai_protocol, addr_str, port
            );
        }
    }

    freeaddrinfo(result);
}
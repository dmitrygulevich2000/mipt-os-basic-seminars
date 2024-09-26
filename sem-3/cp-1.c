#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int write_all(int fd, char* buf, size_t size) {
    printf("sizeof(buf) = %zd\n", sizeof(buf));

    while (size > 0) {
        ssize_t wbytes = write(fd, buf, size);
        if (wbytes < 0) {
            return -1;
        }
        buf += wbytes;
        size -= wbytes;
    }
    return 0;
}

void copy(int sourcefd, int destfd) {
    char buf[1024];
    printf("sizeof(buf) = %zd\n", sizeof(buf));

    ssize_t rbytes = read(sourcefd, buf, sizeof(buf));
    if (rbytes < 0) {
        err(1, "read error at %d", sourcefd);   
    }

    while (rbytes > 0) {    
        ssize_t wbytes = write_all(destfd, buf, rbytes);
        if (wbytes < 0) {
            perror("write_all error");
            exit(1);
        }
        rbytes = read(sourcefd, buf, sizeof(buf));
        if (rbytes < 0) {
            err(1, "read error at %d", sourcefd);
        }
    }
}

int main(int argc, char** argv) {
    printf("argv[0] = %s\n", argv[0]);

    int sourcefd = open(argv[1], O_RDONLY);
    if (sourcefd < 0) {
        fprintf(stderr, "open source error: %d\n", errno);
        exit(1);
    }
    
    int destfd = open(argv[2], O_WRONLY|O_TRUNC|O_CREAT, 0764);
    if (destfd < 0) {
        fprintf(stderr, "open destination error: %s\n", strerror(errno));
        exit(1);
    }
    
    // simulate io error
    // close(sourcefd);
    // close(destfd);
    copy(sourcefd, destfd);

    close(sourcefd);
    close(destfd);
    return 0;
}

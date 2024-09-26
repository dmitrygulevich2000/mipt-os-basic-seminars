// for basename() function
#define _GNU_SOURCE

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int write_all(int fd, char* buf, size_t size) {
    while (size > 0) {
        int wbytes = write(fd, buf, size);
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

    ssize_t rbytes = read(sourcefd, buf, sizeof(buf));
    if (rbytes < 0) {
        err(1, "read error");   
    }

    while (rbytes > 0) {    
        ssize_t wbytes = write_all(destfd, buf, rbytes);
        if (wbytes < 0) {
            perror("write_all error");
            exit(1);
        }
        rbytes = read(sourcefd, buf, sizeof(buf));
        if (rbytes < 0) {
            err(1, "read_error");
        }
    }
}

int prepare_dest(int sourcefd, char* source, char* dest) {
    struct stat source_stat;
    if (fstat(sourcefd, &source_stat) < 0) {
        err(1, "src stat");
        return -1;
    }
    int perm = source_stat.st_mode & 0777;
    
    struct stat dest_stat;
    if (stat(dest, &dest_stat) < 0) {
        if (errno == ENOENT) {
            return open(dest, O_WRONLY|O_CREAT|O_EXCL, perm); 
        }
        return -1;
    }

    char dest_file[PATH_MAX];
    strcpy(dest_file, dest);
    if ((dest_stat.st_mode & S_IFMT) == S_IFDIR) { // S_IFDIR(dest_stat.st_mode)
        char* name = basename(source);
        
        strcat(dest_file, "/");
        strcat(dest_file, name);
    }

    int destfd = open(dest_file, O_WRONLY|O_CREAT|O_TRUNC, perm); 
    return destfd;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        errx(1, "usage:  mycp src_file dst_file\n"
                "or:     mycp src_file dst_dir");
    }

    int sourcefd = open(argv[1], O_RDONLY, 0);
    if (sourcefd < 0) {
        fprintf(stderr, "source open error: %d\n", errno);
        exit(1);
    }
    
    int destfd = prepare_dest(sourcefd, argv[1], argv[2]);
    if (destfd < 0) {
        fprintf(stderr, "prepare destination error: %s\n", strerror(errno));
        exit(1);
    }

    copy(sourcefd, destfd);

    close(sourcefd);
    close(destfd);
    return 0;
}

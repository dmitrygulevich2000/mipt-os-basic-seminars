// for RTLD_NEXT
#define _GNU_SOURCE

#include <assert.h>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>

ssize_t (*real_write)(int, const void*, size_t) = NULL;
int allowed_stdout_fileno = 0;
FILE* allowed_stdout = NULL;

__attribute__((constructor)) void prepare() {
    fprintf(stderr, "loading write.so\n");

    // fixes cat (plain write calls inside executable)
    real_write = dlsym(RTLD_NEXT, "write");
    allowed_stdout_fileno = dup(STDOUT_FILENO);

    // uncommend to fix most bash commands (FILE* I/O)
    // allowed_stdout = fdopen(allowed_stdout_fileno, "a");
    // stdout = allowed_stdout;
}

ssize_t write(int fd, const void* buf, size_t count) {
    int fd_override = fd;
    if (fd == STDOUT_FILENO) {
        fd_override = allowed_stdout_fileno;
    }
    fprintf(stderr, "called write wrapper, fd: %d -> %d\n", fd, fd_override);
    return real_write(fd_override, buf, count);
}

#include <sys/ptrace.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv) {
    // parse args
    if (argc != 3) {
        errx(1, "usage: ./a.out pid address");
    }
    int pid = atoi(argv[1]);
    size_t address = strtoull(argv[2], NULL, 16);

    // open /proc/<pid>/mem
    char mem_path[256];
    sprintf(mem_path, "/proc/%d/mem", pid);
    int fd = open(mem_path, O_RDONLY);
    if (fd < 0) {
        err(1, "open %s", mem_path);
    }

    // attach to process
    int status = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
    if (status < 0) {
        err(1, "ptrace_attach %d", pid);
    }

    // wait for process to be stopped
    int tracee_status = 0;
    pid_t awaited = waitpid(pid, &tracee_status, 0);
    if (awaited < 0) {
        err(1, "waitpid");
    }
    assert(awaited == pid);
    assert(WIFSTOPPED(tracee_status));

    // read from memory
    char buf[256];
    int rbytes = pread(fd, buf, sizeof(buf), address);
    if (rbytes < 0) {
        err(1, "pread at offset %#lx", address);
    }
    printf("valuable information was obtained:\n");
    printf("%.256s", buf);

    // detach & continue process
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
    close(fd);
}

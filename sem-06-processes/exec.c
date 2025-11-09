#define _GNU_SOURCE

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char** argv) {
    printf("starting program\n");
    printf("pid=%d tid=%d\n", getpid(), gettid());

    char* cmd = "mkdir";
    argv[0] = cmd;
    int res = execvp("mkdir", argv);
    // code below executing only if error occurred
    if (res < 0) {
        err(1, "execvp");
    }
    printf("exiting\n");
}

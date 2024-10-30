#include <err.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    printf("starting program\n");

    char* cmd = "mkdir";
    argv[0] = cmd;
    int res = execvp("mkdir", argv);
    // code below executing only if error occurred
    if (res < 0) {
        err(1, "execvp");
    }
    printf("exiting\n");
}


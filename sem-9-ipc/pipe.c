#include <err.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char** argv, char** envs) {
    if (argc == 1) {
        errx(1, "usage: ./a.out command [arg]");
    }

    // create pipe
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        err(1, "pipe");
    }

    // fork
    pid_t pid = fork();
    if (pid < 0) {
        err(1, "fork");
    }
    if (pid == 0) {
        // child:
        // prepare fds
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);

        // exec command
        execvp(argv[1], argv + 1);
        err(1, "execvp");
    }

    // parent:
    // point stdout to pipe
    dup2(pipefd[1], STDOUT_FILENO);

    // print envs
    char** env = envs;
    while (*env != NULL) {
        printf("%s\n", *env);
        ++env;
    }
    
    // optionally: "notify" about end-of-data and wait for child
    fflush(stdout);
    close(STDOUT_FILENO);
    close(pipefd[1]);
    waitpid(pid, NULL, 0);
}

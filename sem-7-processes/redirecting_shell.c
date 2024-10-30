#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void parse_arguments(char* line, char** args, int* out_fd) {
    char* pos;
    pos = strtok(line, " \n");
    int argc = 0;
    while (pos != NULL) {
        if (pos[0] == '>') {
            int fd = open(pos + 1, O_WRONLY|O_CREAT|O_TRUNC, 0664);
            if (fd < 0) {
                perror("failed to redirect output");
            } else {
                *out_fd = fd;
            }
            break;
        }

        args[argc] = pos;
        ++argc;
        pos = strtok(NULL, " \n");
    }
    args[argc] = NULL;
}

int main() {
    char* shell_intro = "my_shell> ";
    printf("%s", shell_intro);

    size_t line_cap = 4096;
    char* line = malloc(line_cap);
    while(getline(&line, &line_cap, stdin) != -1){
        int out_fd = 1;
        char* args[10];
        parse_arguments(line, args, &out_fd);

        pid_t pid = fork();
        if (pid == 0) {
            if (out_fd != 1) {
                dup2(out_fd, 1);
            }
            execvp(args[0], args);
            err(1, "failed to run program");
        }
        if (out_fd != 1) {
            close(out_fd);
        }
        waitpid(pid, NULL, 0);
    
        printf("%s", shell_intro);
    }

    free(line);
    return 0;
}

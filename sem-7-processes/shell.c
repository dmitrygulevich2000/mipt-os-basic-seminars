#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void parse_arguments(char* line, char** args) {
    char* pos;
    pos = strtok(line, " \n");
    int argc = 0;
    while (pos != NULL) {
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
        char* args[10];
        parse_arguments(line, args);

        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
            err(1, "failed to run program");
        }
        waitpid(pid, NULL, 0);
    
        printf("%s", shell_intro);
    }

    free(line);
    return 0;
}

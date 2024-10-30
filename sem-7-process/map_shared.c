#include <err.h>
#include <errno.h>
#include <sched.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    char* private_str = malloc(4096);
    char* shared_str = mmap(
        NULL, 4096, 
        PROT_READ|PROT_WRITE, 
        MAP_ANONYMOUS|MAP_SHARED, -1, 0
    );

    sprintf(private_str, "this private string is written by parent");
    sprintf(shared_str, "this shared string is written by parent");

    pid_t pid = fork();
    if (pid < 0) {
        err(1, "fork");
    }
    if (pid == 0) {
        sprintf(private_str, "this private string is written by child");
        sprintf(shared_str, "this shared string is written by child");
        
        printf("releasing memory in child\n");
        free(private_str); // commenting this lead to memory leak
        if (munmap(shared_str, 4096) < 0) {
            err(1, "parent munmap");
        }
        return 42;
    }

    int status = 0;
    waitpid(pid, &status, 0);
    printf("child %d finished with code %d\n", pid, WEXITSTATUS(status));
    printf("%s\n", private_str);
    printf("%s\n", shared_str);

    printf("releasing memory in parent\n");
    free(private_str); // or this
    if (munmap(shared_str, 4096) < 0) {
        err(1, "parent munmap");
    }
}   

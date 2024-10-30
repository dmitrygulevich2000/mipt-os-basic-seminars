#include <sys/mman.h>
#include <stdio.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        errx(1, "filename not provided");
    }
    printf("pid: %d\n", getpid());
    

    int fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        err(1, "open %s", argv[1]);
    }
    char* file = mmap(NULL, 24, 
        PROT_READ|PROT_WRITE, 
        MAP_SHARED, 
        fd, 0
    );
    if (file == MAP_FAILED) {
        err(1, "mmap");
    }
    printf("mapping address: %p\n", file);


    printf("Mapping created. Press any key to continue.\n");
    fgetc(stdin);

    int counter = 0;
    sscanf(file, "a.out was here %d", &counter);
    sprintf(file, "a.out was here %.2d times\n", counter + 1);

    printf("File changed. Press any key to continue.\n");
    fgetc(stdin);

    close(fd);
    munmap(file, 24);
}

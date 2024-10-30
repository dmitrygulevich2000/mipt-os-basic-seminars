#include <sys/mman.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

int global_int = 10;
int global_int_unitialized;
const int global_const = 42;

int main() {
    printf("pid: %d\n", getpid());
    long page_size = sysconf(_SC_PAGESIZE);
    printf("host page size: %ld B\n", page_size);

    printf("Process started. Press any key to continue.\n");
    fgetc(stdin);

    printf("%llx- program break on start\n", sbrk(0));
    int local_int = 0;
    static int static_int = 10;
    // uncomment below for big_malloc
    void* malloc_mem = malloc(256/**1024*/);
    // uncomment below for mmap_less_than_page
    size_t mmap_length = 2 * page_size/* / 3*/;
    void* mmap_mem = mmap(NULL, 2 * page_size, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
    if (mmap_mem == MAP_FAILED) {
        err(1, "mmap");
    }

    // combine this addresses output with /proc/<pid>/maps output and sort using sort.py
    printf("%llx- global_int\n", &global_int);
    printf("%llx- global_int_unitialized\n", &global_int_unitialized);
    printf("%llx- global_const\n", &global_const);
    printf("%llx- local_int\n", &local_int);
    printf("%llx- malloc_mem\n", malloc_mem);
    printf("%llx- mmap_mem\n", mmap_mem);

    printf("%llx- program break after allocations\n", sbrk(0));

    printf("Memory acquired. Press any key to continue.\n");
    fgetc(stdin);

    int status = munmap(mmap_mem, mmap_length);
    if (status < 0) {
        err(1, "munmap");
    }
    free(malloc_mem);

    printf("Memory released. Press any key to continue.\n");
    fgetc(stdin);
    return 0;
}

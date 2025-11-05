#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    long page_size = sysconf(_SC_PAGESIZE);
    // increase in page count = increase in page faults
    size_t length = 20 * page_size;

    unsigned char* mem =
        mmap(NULL, length, PROT_READ | PROT_WRITE,
             MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (mem == MAP_FAILED) {
        err(1, "mmap");
    }

    unsigned char sum = 0;
    // half steps count -> half increase in page faults
    for (size_t i = 0; i < length / 2; ++i) {
        sum += mem[i];
    }
    printf("sum = %hhu\n", sum);

    munmap(mem, length);
    return 0;
}

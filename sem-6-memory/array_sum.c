#include <sys/mman.h>
#include <stdio.h>
#include <err.h>

int main() {
    size_t size = 0;
    scanf("%ld", &size);
    
    // acquire memory
    int* arr = mmap(
        NULL, size*sizeof(int), 
        PROT_READ|PROT_WRITE, 
        MAP_ANONYMOUS|MAP_PRIVATE, -1, 0
    );
    if (arr == MAP_FAILED) {
        err(1, "mmap");
    }

    // solve task
    for (size_t i = 0; i < size; ++i) {
        scanf("%d", arr + i);
    }
    int sum = 0;
    for (size_t i = 0; i < size; ++i) {
        sum += arr[i];
    }
    printf("%d\n", sum);

    // release memory
    int status = munmap(arr, size*sizeof(int));
    if (status < 0) {
        err(1, "munmap");
    }
}

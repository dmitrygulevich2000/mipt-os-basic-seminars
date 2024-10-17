#include <stdio.h>
#include <inttypes.h>

int64_t sum_result = 1;

extern void array_sum();

int main() {
    array_sum();
    printf("%" PRIi64 "\n", sum_result);
}

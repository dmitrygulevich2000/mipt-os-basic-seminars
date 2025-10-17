#include <inttypes.h>
#include <assert.h>

uint64_t arg1;
int64_t arg2;

uint64_t result;

void func();

void test() {
    arg1 = 5;
    arg2 = 17;
    func();
    assert(result == 100);

    arg1 = 10;
    arg2 = 21;
    func();
    assert(result == 100);

    arg1 = 5;
    arg2 = 20;
    func();
    assert(result == 101);

    arg1 = 11;
    arg2 = 21;
    func();
    assert(result == 101);

    // 0b10..01
    arg1 = (1L << 63) + 1;
    arg2 = 21;
    func();
    assert(result == 101);
}

int main() {
    test();
}


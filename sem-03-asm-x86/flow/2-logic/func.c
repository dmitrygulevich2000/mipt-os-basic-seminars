#include <inttypes.h>

extern uint64_t arg1;
extern int64_t arg2;

extern uint64_t result;

void func() {
    result = 100;
    if (arg1 > 10 || arg2 == 20) {
        result += 1;
    }
}

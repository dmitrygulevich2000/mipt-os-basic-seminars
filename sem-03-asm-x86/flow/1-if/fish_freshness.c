#include <inttypes.h>

extern int64_t days;
extern int64_t freshness;

void fish_freshness() {
    if (days >= 3) {
        freshness = 2;
    } else {
        freshness = 1;
    }
}
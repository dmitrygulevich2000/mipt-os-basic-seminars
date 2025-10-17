#include <inttypes.h>
#include <assert.h>

int64_t days;
int64_t freshness;

void fish_freshness();

int main() {
    days = 5;
    fish_freshness();
    assert(freshness == 2);

    days = 3;
    fish_freshness();
    assert(freshness == 2);

    days = 2;
    fish_freshness();
    assert(freshness == 1);

    days = -1;
    fish_freshness();
    assert(freshness == 1);
}


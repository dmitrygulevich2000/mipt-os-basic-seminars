#include <math.h>
#include <assert.h>

extern float circle_area(float radius);

const float TOL = 1e-5;  // tolerance

int main() {
    assert(fabs(circle_area(2) - 4*M_PI) < TOL);
    assert(fabs(circle_area(3.3) - 3.3*3.3*M_PI) < TOL);
    assert(fabs(circle_area(10) - 100*M_PI) < TOL);
}

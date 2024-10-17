#include <immintrin.h>

// intrinsics reference: https://www.laruence.com/sse/#
//
float array_avg_intrin(float* array, size_t size) {
    float sum = 0;
    __m256 counters = _mm256_setzero_ps();
    for (size_t i = 0; i < size; i += 8) {
        __m256 batch = _mm256_load_ps(array + i); // or loadu
        counters = _mm256_add_ps(counters, batch);
    }

    __m128 higher_half = _mm256_extractf128_ps(counters, 1);
    __m128 lower_half = _mm256_extractf128_ps(counters, 0);;
    __m128 rsum = _mm_hadd_ps(higher_half, lower_half);
    rsum = _mm_hadd_ps(rsum, rsum);
    rsum = _mm_hadd_ps(rsum, rsum);

    _mm_store_ss(&sum, rsum);
    return sum / size;
}

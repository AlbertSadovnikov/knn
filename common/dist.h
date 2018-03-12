#pragma once

#include <array>
#include <vector>
#include <assert.h>

template<class T>
T euclidean_distance(const std::vector<T>& a, const std::vector<T>& b) {
    assert(a.size()==b.size());
    T ret = T(0);
    for(size_t i = 0; i < a.size(); i++) {
        ret += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return std::sqrt(ret);
}

template<class T>
T dist(const std::vector<T>& a, const std::vector<T>& b) {
    return euclidean_distance<T>(a, b);
}

template<class T, size_t D>
T euclidean_optimized(const std::vector<T>& a, const std::vector<T>& b) {
    std::array<T, D> buffer;
    for(size_t i = 0; i < D; i++) {
        const auto x = a[i] - b[i];
        buffer[i] = x * x;
    }
    return std::sqrt(std::accumulate(std::begin(buffer), std::begin(buffer) + D, T(0.0)));
};

template<size_t D>
static inline float euclidean_intrinsic_float(const float* x, const float* y) {
    float result=0;
    __m128 euclidean = _mm_setzero_ps();
    for (size_t k = 0; k < D; k += 4) {
        const __m128 a = _mm_loadu_ps(x);
        const __m128 b = _mm_loadu_ps(y);
        const __m128 a_minus_b = _mm_sub_ps(a,b);
        const __m128 a_minus_b_sq = _mm_mul_ps(a_minus_b, a_minus_b);
        euclidean = _mm_add_ps(euclidean, a_minus_b_sq);
        x+=4;
        y+=4;
    }
    const __m128 shuffle1 = _mm_shuffle_ps(euclidean, euclidean, _MM_SHUFFLE(1,0,3,2));
    const __m128 sum1 = _mm_add_ps(euclidean, shuffle1);
    const __m128 shuffle2 = _mm_shuffle_ps(sum1, sum1, _MM_SHUFFLE(2,3,0,1));
    const __m128 sum2 = _mm_add_ps(sum1, shuffle2);
    _mm_store_ss(&result,sum2);
    return result;
}
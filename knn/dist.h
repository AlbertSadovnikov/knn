#pragma once

#include <array>
#include <vector>

namespace knn {
    template<uint32_t V>
    inline float distance(const float* a, const float* b) {
        float acc = 0;
        for (uint32_t i = 0; i < V; i++) {
            const auto d = a[i] - b[i];
            acc += d * d;
        }
        return sqrtf(acc);
    }

    template<>
    inline float distance<512>(const float* a, const float* b) {
        float result = 0;
        __m128 euclidean = _mm_setzero_ps();
        for (size_t k = 0; k < 512; k += 4) {
            const __m128 a4 = _mm_loadu_ps(a);
            const __m128 b4 = _mm_loadu_ps(b);
            const __m128 a_minus_b = _mm_sub_ps(a4, b4);
            const __m128 a_minus_b_sq = _mm_mul_ps(a_minus_b, a_minus_b);
            euclidean = _mm_add_ps(euclidean, a_minus_b_sq);
            a += 4;
            b += 4;
        }
        const __m128 shuffle1 = _mm_shuffle_ps(euclidean, euclidean, _MM_SHUFFLE(1, 0, 3, 2));
        const __m128 sum1 = _mm_add_ps(euclidean, shuffle1);
        const __m128 shuffle2 = _mm_shuffle_ps(sum1, sum1, _MM_SHUFFLE(2, 3, 0, 1));
        const __m128 sum2 = _mm_add_ps(sum1, shuffle2);
        _mm_store_ss(&result, sum2);
        return sqrtf(result);
    }
}
#pragma once

#include <cstdint>
#include <cmath>
#include <limits>
#include "dist.h"


namespace knn {

// knn routines
// in order insertion
// val  - value to insert
// index - corresponding index
// dist - distances array
// idx - indexes array
template<uint32_t K>
inline void insert_in_order(float val, uint32_t index, float* dist, uint32_t* idx);

template<>
inline void insert_in_order<1>(float val, uint32_t index, float* dist, uint32_t* idx) {
    if (val < *dist) {
        *idx = index;
        *dist = val;
    }
}

template<>
inline void insert_in_order<2>(float val, uint32_t index, float* dist, uint32_t* idx) {
    if (val < dist[0]) {
        idx[1] = idx[0]; idx[0] = index;
        dist[1] = dist[0]; dist[0] = val;
        return;
    }
    if (val < dist[1]) {
        idx[1] = index;
        dist[1] = val;
    }
}

// base template
// V - vector size
// N - number of vectors in lookup table
// Q - number of vectors in a query
// K - number of nearest neighbours per query to output
// data - input data [V * N]
// query - query data [V * Q]
// idx - sorted response indexes [Q * K], i.e. Q blocks of K indexes in ascending order
// dist - sorted response distances [Q * K], i.e. Q blocks of K distances in ascending order


template<uint32_t V, uint32_t N, uint32_t Q, uint32_t K>
void knn_base(float* data, float* query, uint32_t* idx, float* dist) {

    for (uint32_t q = 0; q < Q; q++) {
        float* local_query = query + V * q;
        float* local_dist = dist + K * q;
        uint32_t* local_idx = idx + K * q;
        // fill up distances with max float
        for (uint32_t k = 0; k < K; k++) {
            local_dist[k] = std::numeric_limits<float>::max();
        }
        for (uint32_t n = 0; n < N; n++) {
            const auto d = distance<V>(local_query, data + V * n);
            insert_in_order<K>(d, n, local_dist, local_idx);
        }
    }
}
}
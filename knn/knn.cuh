#pragma once
#include <cstdint>
#include <iostream>


namespace knn {
    void printCudaVersion();
    void printCudaDevices();

    void knn_gpu_test(unsigned gpu_index,
                      uint32_t V, uint32_t N, uint32_t Q, uint32_t K,
                      float* data, float* query, uint32_t* idx, float* dist);

}
#pragma once


namespace knn {
    void printCudaVersion();
    void printCudaDevices();

    void knn_gpu_test(unsigned gpu_index,
                      unsigned V, unsigned N, unsigned Q, unsigned K,
                      float* data, float* query, unsigned* idx, float* dist);

}
#include "knn.cuh"
#include <iostream>

using namespace std;

namespace knn {

void printCudaVersion() {
    int runtime_ver;
    cudaRuntimeGetVersion(&runtime_ver);
    cout << "CUDA Runtime version: " << runtime_ver << '\n';

    int driver_ver;
    cudaDriverGetVersion(&driver_ver);
    std::cout << "CUDA Driver version: " << driver_ver << '\n';
}

void printCudaDevices() {
    int devicesCount;
    cudaGetDeviceCount(&devicesCount);


    for(int deviceIndex = 0; deviceIndex < devicesCount; ++deviceIndex) {
        cudaDeviceProp deviceProperties;
        cudaGetDeviceProperties(&deviceProperties, deviceIndex);
        cout << "gpu[" << deviceIndex << "]: " << deviceProperties.name
             << " (" << deviceProperties.major << "." << deviceProperties.minor << ")\n";
    }
}


__global__ void knn_gpu(unsigned V, unsigned N, unsigned Q, unsigned K,
                        float* data, float* query, unsigned* idx, float* dist) {

}



void knn_gpu_test(unsigned gpu_index,
                  unsigned V, unsigned N, unsigned Q, unsigned K,
                  float* data, float* query, unsigned* idx, float* dist) {

    // get devices count
    int devicesCount;
    cudaGetDeviceCount(&devicesCount);
    if (gpu_index >= devicesCount) {
        std::cout << "GPU index [" << gpu_index << "] out of range!" << '\n';
        return;
    }

    cudaDeviceProp deviceProperties;
    cudaGetDeviceProperties(&deviceProperties, gpu_index);
    cudaSetDevice(gpu_index);
    cout << "Using gpu[" << gpu_index << "]: " << deviceProperties.name
         << " (" << deviceProperties.major << "." << deviceProperties.minor << ")\n";

    // copy data to device
    float* d_data;
    cudaMalloc((void**)&d_data, V * N * sizeof(float));
    cudaMemcpy(d_data, data, V * N * sizeof(float), cudaMemcpyHostToDevice);
    // copy query to device
    float* d_query;
    cudaMalloc((void**)&d_query, V * Q * sizeof(float));
    cudaMemcpy(d_query, query, V * Q * sizeof(float), cudaMemcpyHostToDevice);
    // allocate indexes
    unsigned* d_idx;
    cudaMalloc((void**)&d_idx, K * Q * sizeof(unsigned));
    // allocate distances
    float* d_dist;
    cudaMalloc((void**)&d_dist, K * Q * sizeof(float));



    // free
    cudaFree(d_dist);
    cudaFree(d_idx);
    cudaFree(d_query);
    cudaFree(d_data);


/*


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
        }*/
}


}

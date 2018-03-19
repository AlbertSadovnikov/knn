cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CUDA_COMPILER /usr/local/cuda-9.1/bin/nvcc)

project(knn LANGUAGES CUDA CXX)


# boost dependency
set(BOOST_ROOT /opt/boost)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.66 COMPONENTS program_options filesystem REQUIRED)


add_subdirectory(proto)
add_subdirectory(common)
add_subdirectory(knn)
add_subdirectory(datagen)
add_subdirectory(benchmark)

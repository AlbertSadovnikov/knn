
set(KNN_HDR knn.h dist.h)
set(KNN_SRC knn.cpp)
set(KNN_CUDA_SRC knn.cu knn.cuh)

add_library(knn STATIC ${KNN_SRC} ${KNN_HDR} ${KNN_CUDA_SRC})
target_compile_features(knn PUBLIC cxx_std_11)
set_target_properties(knn PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_include_directories(knn PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

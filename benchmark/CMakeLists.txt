
set(BENCHMARK_SRC main.cpp)

add_executable(benchmark ${BENCHMARK_SRC})

target_link_libraries(benchmark proto common knn Boost::boost)
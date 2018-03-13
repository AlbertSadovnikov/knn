
set(KNN_HDR knn.h)
set(KNN_SRC knn.cpp)

add_library(knn STATIC ${KNN_SRC} ${KNN_HDR})
target_include_directories(knn PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

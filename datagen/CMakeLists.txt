
find_package(OpenMP REQUIRED)

if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(BOOST_ROOT /opt/boost)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.66 COMPONENTS program_options filesystem REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

set(DATAGEN_SRC main.cpp)

add_executable(datagen ${DATAGEN_SRC})
target_link_libraries(datagen proto common knn ${Boost_LIBRARIES})
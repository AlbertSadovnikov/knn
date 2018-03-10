

set(BOOST_ROOT /opt/boost)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.66 COMPONENTS program_options REQUIRED)

include_directories(${Boost_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/common)

set(DATAGEN_SRC main.cpp)

add_executable(datagen ${DATAGEN_SRC})

target_link_libraries(datagen proto ${Boost_LIBRARIES})
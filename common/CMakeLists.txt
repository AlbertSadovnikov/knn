

set(COMMON_HDR serialize.h dist.h rand.h)
set(COMMON_SRC serialize.cpp)

add_library(common STATIC ${COMMON_SRC} ${COMMON_HDR})
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(common proto ${Boost_LIBRARIES})
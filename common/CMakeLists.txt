

set(COMMON_HDR rand.h serialize.h)
set(COMMON_SRC)

add_library(common STATIC ${COMMON_SRC} ${COMMON_HDR})
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(common proto ${Boost_LIBRARIES})
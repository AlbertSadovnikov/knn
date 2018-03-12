
# optional protobuf
set(Protobuf_USE_STATIC_LIBS ON)
set(Protobuf_INCLUDE_DIR /opt/google/protobuf/include)
set(Protobuf_PROTOC_EXECUTABLE /opt/google/protobuf/bin/protoc)
set(Protobuf_LIBRARY /opt/google/protobuf/lib/libprotobuf.a)
set(Protobuf_LITE_LIBRARY /opt/google/protobuf/lib/libprotobuf-lite.so)

find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS data.proto)
add_library(proto STATIC ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(proto ${Protobuf_LIBRARY})
target_include_directories(proto PUBLIC ${Protobuf_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
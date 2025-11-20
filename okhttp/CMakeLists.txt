cmake_minimum_required(VERSION 3.16)
project(grpc_helloworld C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# gRPC C++ 플러그인 경로 찾기
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)

add_library(protolib helloworld.proto)
target_link_libraries(protolib gRPC::grpc++)
target_include_directories(protolib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# 일반 protobuf 파일 생성 (.pb.h, .pb.cc)
protobuf_generate(TARGET protolib LANGUAGE cpp)

# gRPC 서비스 파일 생성 (.grpc.pb.h, .grpc.pb.cc)
protobuf_generate(
    TARGET protolib 
    LANGUAGE grpc 
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
    PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
)

add_executable(server server.cc)
target_link_libraries(server protolib)

add_executable(client client.cc)
target_link_libraries(client protolib)
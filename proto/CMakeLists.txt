find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

project(proto)

set(PROTO_FILES
    protos/file.proto
)

add_library(${PROJECT_NAME} ${PROTO_FILES})
target_link_libraries(${PROJECT_NAME}
    PUBLIC 
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
)
target_include_directories(
    ${PROJECT_NAME} 
    PUBLIC 
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/protos
)

get_target_property(
    grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION
)
protobuf_generate(TARGET ${PROJECT_NAME} LANGUAGE cpp)
protobuf_generate(
    TARGET ${PROJECT_NAME} 
    LANGUAGE grpc 
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc 
    PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
)

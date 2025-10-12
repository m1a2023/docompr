cmake_minimum_required(VERSION 3.10)

project(docompr 
    VERSION 0.0.1 
    DESCRIPTION "File compressor microservice for docshell"
)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_BUILD_TYPE STREQUAL "Debug") 
    message(
        "Building ${PROJECT_NAME}:
            Version: ${PROJECT_VERSION};
            Type: ${CMAKE_BUILD_TYPE};
            CXX: ${CMAKE_CXX_STANDARD};"
    )
endif()

#
# Executable client
#

add_subdirectory(server)
add_subdirectory(client)

#
# Add zlib and gzstream libs 
# to compress files  
#
add_subdirectory(libs)

#
# Add grpc and protobuf
#
add_subdirectory(proto)
cmake_minimum_required(VERSION 3.10)

project(docompr)

set(CMAKE_CXX_STANDARD 17)


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
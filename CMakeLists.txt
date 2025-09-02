cmake_minimum_required(VERSION 3.10)

project(docompr)

set(CMAKE_CXX_STANDARD 17)


#
# Executable client
#

add_subdirectory(server)
add_subdirectory(client)

# set(SOURCES
#     client/src/main.cpp
# )

# add_executable(${PROJECT_NAME}_client
#     client/src/main.cpp
# )

# set(SOURCES
#     server/src/main.cpp
# )

# add_executable(${PROJECT_NAME}_server
#     server/src/main.cpp
# )

#
# Add zlib and gzstream libs 
# to compress files  
#
add_subdirectory(libs)

# add_dependencies(docompr build_external_libs)

# target_include_directories(docompr PUBLIC
#     ${CMAKE_SOURCE_DIR}/libs/zlib
#     ${CMAKE_SOURCE_DIR}/libs/gzstream
# )

# target_link_libraries(${PROJECT_NAME} 
#     PRIVATE 
#         gzstream
#         z
# )


#
# Add grpc and protobuf
#
add_subdirectory(proto)

# target_link_libraries(${PROJECT_NAME}
#     PRIVATE proto_client
# )
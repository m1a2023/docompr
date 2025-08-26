cmake_minimum_required(VERSION 3.10)
project(docompr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTO_MOC ON)

add_subdirectory(libs)

add_executable(${PROJECT_NAME} 
    main.cpp
)

add_dependencies(${PROJECT_NAME} build_external_libs)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/libs/zlib
    ${CMAKE_SOURCE_DIR}/libs/gzstream
)

link_directories(${CMAKE_BINARY_DIR}/libs)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        gzstream
        z
)

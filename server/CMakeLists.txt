set(SERVER_SOURCES 
    src/main.cpp
    src/Compressor.cpp
)

add_executable(server ${SERVER_SOURCES})
add_dependencies(server build_external_libs)
target_link_libraries(server
    PRIVATE 
        proto
        gzstream
        z
)
target_include_directories(server PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libs/zlib
    ${CMAKE_SOURCE_DIR}/libs/gzstream
)


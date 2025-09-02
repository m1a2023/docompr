set(CLIENT_SOURCES 
    src/main.cpp
)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SOURCES})

add_executable(client ${CLIENT_SOURCES})
add_dependencies(client build_external_libs)
target_link_libraries(client
    PRIVATE 
        proto
        gzstream
        z
)
target_include_directories(client PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libs/zlib
    ${CMAKE_SOURCE_DIR}/libs/gzstream
)

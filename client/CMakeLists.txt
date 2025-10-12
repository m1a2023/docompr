set(CLIENT_SOURCES 
    test/main.cpp
)

add_executable(client ${CLIENT_SOURCES})
add_dependencies(client build_external_libs)
target_link_libraries(client
    PRIVATE proto
)
target_include_directories(client PUBLIC
    ${CMAKE_SOURCE_DIR}/test
)

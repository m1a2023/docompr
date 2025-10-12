
message(${CMAKE_CURRENT_SOURCE_DIR})
find_package(Boost REQUIRED COMPONENTS iostreams)

set(SERVER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CompressorImpl.cpp
)

add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Sources: ${SERVER_SOURCES}")
endif()


add_executable(server ${SERVER_SOURCES})
add_dependencies(server build_external_libs)

target_link_libraries(server
    PRIVATE 
        proto 
        z
        Boost::iostreams
)

target_include_directories(server PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libs/zlib
    ${Boost_INCLUDE_DIRS}
)


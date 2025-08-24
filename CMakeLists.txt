cmake_minimum_required(VERSION 4.0)
project(docompr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTO_MOC ON)


include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    gzstream z
)
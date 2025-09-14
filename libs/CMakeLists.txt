set(LIBS_OUT_DIR ${CMAKE_BINARY_DIR}/libs)

# ---------- ZLIB ----------
set(ZLIB_SRC_DIR ${CMAKE_SOURCE_DIR}/libs/zlib)
set(ZLIB_STATIC_SRC ${ZLIB_SRC_DIR}/libz.a)
set(ZLIB_SHARED_SRC ${ZLIB_SRC_DIR}/libz.so)

set(ZLIB_STATIC ${LIBS_OUT_DIR}/libz.a)
set(ZLIB_SHARED ${LIBS_OUT_DIR}/libz.so)

add_custom_command(
    OUTPUT ${ZLIB_STATIC} ${ZLIB_SHARED}
    COMMAND ${CMAKE_COMMAND} -E rm -rf build
    COMMAND ${CMAKE_COMMAND} -E make_directory build
    COMMAND ${CMAKE_COMMAND} -E chdir build ${CMAKE_COMMAND} ..
    COMMAND ${CMAKE_COMMAND} --build build
    COMMAND ${CMAKE_COMMAND} -E copy_if_different build/libz.a ${ZLIB_STATIC}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different build/libz.so ${ZLIB_SHARED}
    WORKING_DIRECTORY ${ZLIB_SRC_DIR}
    COMMENT "Compiling zlib library"
)

add_custom_target(build_zlib
    DEPENDS ${ZLIB_STATIC} ${ZLIB_SHARED}
    COMMENT "Target compilation library"
)
add_library(z STATIC IMPORTED)
set_target_properties(z PROPERTIES IMPORTED_LOCATION ${ZLIB_STATIC})

# ---------- GZSTREAM ----------
set(GZSTREAM_SRC_DIR ${CMAKE_SOURCE_DIR}/libs/gzstream)
set(GZSTREAM_STATIC_SRC ${GZSTREAM_SRC_DIR}/libgzstream.a)

set(GZSTREAM_STATIC ${LIBS_OUT_DIR}/libgzstream.a)

add_custom_command(
    OUTPUT ${GZSTREAM_STATIC}
    COMMAND make
    COMMAND ${CMAKE_COMMAND} -E make_directory ${LIBS_OUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GZSTREAM_STATIC_SRC} ${GZSTREAM_STATIC}
    COMMAND make build_static
    WORKING_DIRECTORY ${GZSTREAM_SRC_DIR}
    COMMENT "Compiling gzstream library"
)

add_custom_target(build_gzstream
    DEPENDS ${GZSTREAM_STATIC}
    COMMENT "Target compilation library"
)

add_library(gzstream STATIC IMPORTED)
set_target_properties(gzstream PROPERTIES IMPORTED_LOCATION ${GZSTREAM_STATIC})

# ---------- ALL ----------
add_custom_target(build_external_libs
    DEPENDS build_zlib build_gzstream
    COMMENT "Building all external libs"
)

link_directories(${CMAKE_BINARY_DIR}/libs)


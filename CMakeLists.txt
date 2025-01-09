cmake_minimum_required(VERSION 3.31.0)

project(SnifferProject)

set(EXTERNAL_LIB ${CMAKE_SOURCE_DIR}/vendor)

enable_testing()
add_subdirectory(vendor/Catch2)

set(LIBDESTDIR ${CMAKE_SOURCE_DIR}/LGSniffer/lib)
add_custom_target(build_libnl ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/vendor/libnl_build
    COMMAND [ -f ${CMAKE_BINARY_DIR}/vendor/libnl_build/Makefile ] || (cd ${CMAKE_BINARY_DIR}/vendor/libnl_build && ${EXTERNAL_LIB}/libnl/autogen.sh)
    COMMAND [ -f ${CMAKE_BINARY_DIR}/vendor/libnl_build/Makefile ] || (cd ${CMAKE_BINARY_DIR}/vendor/libnl_build && ${EXTERNAL_LIB}/libnl/configure --prefix=/usr --disable-static)
    COMMAND make -C ${CMAKE_BINARY_DIR}/vendor/libnl_build
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/vendor/libnl_build/lib/.libs/libnl-3.so ${LIBDESTDIR}/libnl-3.so
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/vendor/libnl_build/lib/.libs/libnl-genl-3.so ${LIBDESTDIR}/libnl-genl-3.so
    COMMENT "Configuring and building libnl"
)

add_subdirectory(libsniffer)
add_subdirectory(serialSniffer)
add_subdirectory(tests)

add_custom_target(
    build_modules ALL
    COMMAND make -C ${CMAKE_SOURCE_DIR}/modules
    COMMENT "Building modules using Makefile in 'modules' directory"
)
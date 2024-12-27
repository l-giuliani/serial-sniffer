cmake_minimum_required(VERSION 3.31.0)

project(SnifferProject)

add_subdirectory(libsniffer)
add_subdirectory(serialSniffer)

enable_testing()
add_subdirectory(vendor/Catch2)
add_subdirectory(tests)

add_custom_target(
    build_modules ALL
    COMMAND make -C ${CMAKE_SOURCE_DIR}/modules
    COMMENT "Building modules using Makefile in 'modules' directory"
)
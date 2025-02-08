cmake_minimum_required(VERSION 3.31.0)

project(SerialSniffer
        VERSION 0.9.0
        DESCRIPTION "Serial Sniffer")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES "*.cpp" "**/*.cpp" "**/**/*.cpp" "**/**/**/*.cpp")

file(GLOB_RECURSE HEADERS "*.h" "**/*.h" "**/**/*.h" "**/**/**/*.h")
set(HEADER_DIRECTORIES)
foreach(header IN LISTS HEADERS)
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND HEADER_DIRECTORIES ${dir})
endforeach()

list(APPEND HEADER_DIRECTORIES ${CMAKE_SOURCE_DIR}/libsniffer/src)

add_executable(SerialSniffer ${SOURCES})

target_include_directories(SerialSniffer PRIVATE ${HEADER_DIRECTORIES})

set(LIBDESTDIR ${CMAKE_SOURCE_DIR}/LGSniffer/lib)
target_link_libraries(SerialSniffer 
                ${LIBDESTDIR}/libsniffer.so
                ${LIBDESTDIR}/libnl-genl-3.so 
                ${LIBDESTDIR}/libnl-3.so)

add_custom_command(TARGET SerialSniffer 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SerialSniffer> ${CMAKE_SOURCE_DIR}/LGSniffer/bin)
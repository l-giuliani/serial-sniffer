cmake_minimum_required(VERSION 3.31.0)

project(SerialSniffer
        VERSION 0.9.0
        DESCRIPTION "Serial Sniffer")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "*.cpp" "**/*.cpp")

file(GLOB_RECURSE HEADERS "*.h" "**/*.h")
set(HEADER_DIRECTORIES)
foreach(header IN LISTS HEADERS)
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND HEADER_DIRECTORIES ${dir})
endforeach()

list(APPEND HEADER_DIRECTORIES ${CMAKE_SOURCE_DIR}/libsniffer/src)

add_executable(SerialSniffer ${SOURCES})

target_include_directories(SerialSniffer PRIVATE ${HEADER_DIRECTORIES})

target_link_libraries(SerialSniffer 
                ${CMAKE_SOURCE_DIR}/bin/libsniffer.so
                ${CMAKE_SOURCE_DIR}/bin/libnl-genl-3.so 
                ${CMAKE_SOURCE_DIR}/bin/libnl-3.so)

add_custom_command(TARGET SerialSniffer 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:SerialSniffer> ${CMAKE_SOURCE_DIR}/bin)
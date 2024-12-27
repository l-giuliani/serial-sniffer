cmake_minimum_required(VERSION 3.31.0)

project(sniffer
        VERSION 0.9.0
        DESCRIPTION "Serial sniffer lib")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/**/*.cpp")

file(GLOB_RECURSE HEADERS "src/*.h" "src/**/*.h")
set(HEADER_DIRECTORIES)
foreach(header IN LISTS HEADERS)
    get_filename_component(dir ${header} DIRECTORY)
    list(APPEND HEADER_DIRECTORIES ${dir})
endforeach()


add_library(sniffer SHARED ${SOURCES})

target_include_directories(sniffer PRIVATE ${HEADER_DIRECTORIES})

add_custom_command(TARGET sniffer 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:sniffer> ${CMAKE_SOURCE_DIR}/bin)
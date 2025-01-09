file(GLOB_RECURSE TEST_SOURCES "integration/**/*.cpp" "unit/**/*.cpp")
add_executable(snifferTests ${TEST_SOURCES})

set(LIBDESTDIR ${CMAKE_SOURCE_DIR}/LGSniffer/lib)
target_link_libraries(snifferTests PRIVATE Catch2::Catch2WithMain
                    ${LIBDESTDIR}/libsniffer.so
                    ${LIBDESTDIR}/libnl-genl-3.so 
                    ${LIBDESTDIR}/libnl-3.so)

include(CTest)
include(Catch)
catch_discover_tests(snifferTests)

list(APPEND HEADER_DIRECTORIES ${CMAKE_SOURCE_DIR}/libsniffer/src
                ${CMAKE_SOURCE_DIR}/libsniffer/src/libs)

list(APPEND HEADER_DIRECTORIES ${CMAKE_SOURCE_DIR}/vendor/libnl/include)
list(APPEND HEADER_DIRECTORIES ${CMAKE_BINARY_DIR}/vendor/libnl_build/include)
list(APPEND HEADER_DIRECTORIES ${CMAKE_SOURCE_DIR}/modules/serial-bridge/include)
target_include_directories(snifferTests PRIVATE ${HEADER_DIRECTORIES})

add_custom_command(TARGET snifferTests 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:snifferTests> ${CMAKE_SOURCE_DIR}/LGSniffer/bin)
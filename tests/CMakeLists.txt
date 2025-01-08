file(GLOB_RECURSE TEST_SOURCES "integration/**/*.cpp" "unit/**/*.cpp")
add_executable(snifferTests ${TEST_SOURCES})

target_link_libraries(snifferTests PRIVATE Catch2::Catch2WithMain
                    ${CMAKE_SOURCE_DIR}/bin/libsniffer.so
                    ${CMAKE_SOURCE_DIR}/bin/libnl-genl-3.so 
                    ${CMAKE_SOURCE_DIR}/bin/libnl-3.so)

include(CTest)
include(Catch)
catch_discover_tests(snifferTests)

list(APPEND HEADER_DIRECTORIES ${CMAKE_SOURCE_DIR}/libsniffer/src
                ${CMAKE_SOURCE_DIR}/libsniffer/src/libs)
target_include_directories(SerialSniffer PRIVATE ${HEADER_DIRECTORIES})

add_custom_command(TARGET snifferTests 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:snifferTests> ${CMAKE_SOURCE_DIR}/bin/tests)
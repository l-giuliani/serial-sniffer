file(GLOB_RECURSE TEST_SOURCES "integration/**/*.cpp" "unit/**/*.cpp" "unit/*.cpp")
list(APPEND TEST_SOURCES ${CMAKE_SOURCE_DIR}/shared/utils/c_timer.cpp)
add_executable(snifferTests ${TEST_SOURCES})

set(LIBDESTDIR ${CMAKE_SOURCE_DIR}/LGSniffer/lib)
target_link_libraries(snifferTests PRIVATE Catch2::Catch2WithMain
                    ${LIBDESTDIR}/libsniffer.so
                    ${LIBDESTDIR}/libnl-genl-3.so 
                    ${LIBDESTDIR}/libnl-3.so
                    pthread)

include(CTest)
include(Catch)
catch_discover_tests(snifferTests)

list(APPEND HEADER_DIRECTORIES ${CMAKE_SOURCE_DIR}/libsniffer/src
                ${CMAKE_SOURCE_DIR}/libsniffer/src/libs)

list(APPEND HEADER_DIRECTORIES ${CMAKE_SOURCE_DIR}/vendor/libnl/include)
list(APPEND HEADER_DIRECTORIES ${CMAKE_BINARY_DIR}/vendor/libnl_build/include)
list(APPEND HEADER_DIRECTORIES ${CMAKE_SOURCE_DIR}/modules/serial-bridge/include
                            ${CMAKE_SOURCE_DIR}/shared/utils)
target_include_directories(snifferTests PRIVATE ${HEADER_DIRECTORIES})

add_custom_command(TARGET snifferTests 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:snifferTests> ${CMAKE_SOURCE_DIR}/LGSniffer/bin)

#Helpers

# list(APPEND HELPER_SOURCES integration/libsniffer/helpers/libsnifferTestHelper.cpp)      
# add_executable(libsnifferTestHelper ${HELPER_SOURCES})

# target_link_libraries(libsnifferTestHelper
#                     ${LIBDESTDIR}/libsniffer.so
#                     ${LIBDESTDIR}/libnl-genl-3.so 
#                     ${LIBDESTDIR}/libnl-3.so)     
                    
# target_include_directories(libsnifferTestHelper PRIVATE ${HEADER_DIRECTORIES})

# add_custom_command(TARGET libsnifferTestHelper 
#                    POST_BUILD
#                    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:libsnifferTestHelper> ${CMAKE_SOURCE_DIR}/LGSniffer/test)

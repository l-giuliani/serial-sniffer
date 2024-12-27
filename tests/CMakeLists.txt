add_executable(snifferTests test_main.cpp)
target_link_libraries(snifferTests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(snifferTests)

add_custom_command(TARGET snifferTests 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:snifferTests> ${CMAKE_SOURCE_DIR}/bin/tests)
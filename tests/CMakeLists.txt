enable_testing()
add_executable(hello_cs51test hello_test.cpp)
target_link_libraries(hello_cs51test PRIVATE lab1 GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(hello_cs51test)



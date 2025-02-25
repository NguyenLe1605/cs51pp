enable_testing()

# TODO: add a for loop to run test
add_executable(hello_cs51test hello_test.cpp)
target_link_libraries(hello_cs51test PRIVATE lab1 GTest::gtest_main)

add_executable(lab1_tests lab1_tests.cpp)
target_link_libraries(lab1_tests PRIVATE lab1 GTest::gtest_main)

add_executable(ps1_tests ps1_tests.cpp)
target_link_libraries(ps1_tests PRIVATE ps1 GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(hello_cs51test)
gtest_discover_tests(lab1_tests)
gtest_discover_tests(ps1_tests)

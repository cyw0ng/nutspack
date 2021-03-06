add_executable(
    ut_test_common
    logging_test.cc
)

target_link_libraries(
    ut_test_common
    gtest
    pthread
)
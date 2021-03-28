add_executable(
    ut_test_common
    logging_test.cc
    database_test.cc
)

target_link_libraries(
    ut_test_common
    gtest_main
    gtest
    pthread
    sqlite3
    np_logging
    np_database
)
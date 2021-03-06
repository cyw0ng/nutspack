execute_process (
    COMMAND bash -c "CC=clang-10 CXX=clang++-10 cmake -DBUILD_SHARED_LIBS=ON .; make -j $NPROC"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/googletest
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/googletest/lib/libgtest.so ${CMAKE_CURRENT_SOURCE_DIR}/googletest/lib/libgtest_main.so
    DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} FOLLOW_SYMLINK_CHAIN)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include/gtest
    DESTINATION ${NP_HEADERS_OUTPUT_DIRECTORY})

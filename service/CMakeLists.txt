project (nutspack-service)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output/bin)
set(NP_HEADERS_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output/include)

add_subdirectory(thirdparty)
add_subdirectory(src)
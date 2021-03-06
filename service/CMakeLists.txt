project (nutspack-service)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output/bin)
set(NP_HEADERS_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/output/include)

option(TEST "build nutspack service with tests" OFF)

add_subdirectory(thirdparty)
add_subdirectory(src)

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
include_directories(${NP_HEADERS_OUTPUT_DIRECTORY})

if (TEST)
    add_subdirectory(test)
endif (TEST)
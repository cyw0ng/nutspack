add_library(np_foundation SHARED
    Foundation.cc
)

file(COPY Foundation.hh DESTINATION ${NP_HEADERS_OUTPUT_DIRECTORY}/common/Foundation)

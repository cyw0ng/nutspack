add_library(np_logging SHARED
    logging.cc
)

file(COPY logging.hh DESTINATION ${NP_HEADERS_OUTPUT_DIRECTORY}/common/logging)
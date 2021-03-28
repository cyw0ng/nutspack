add_library(np_database SHARED
    database.cc
)

file(COPY database.hh DESTINATION ${NP_HEADERS_OUTPUT_DIRECTORY}/common/database)
add_executable(tst-request-parser EXCLUDE_FROM_ALL tst-request-parser.cc)
target_link_libraries(tst-request-parser mimosa-http gtest pthread)
add_executable(hello-opts EXCLUDE_FROM_ALL hello-opts.cc)
target_link_libraries(hello-opts mimosa-options)

add_executable(duplicate-opts EXCLUDE_FROM_ALL duplicate-opts.cc)
target_link_libraries(duplicate-opts mimosa-options)

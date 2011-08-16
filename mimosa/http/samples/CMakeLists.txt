add_executable(web-server EXCLUDE_FROM_ALL web-server.cc)
target_link_libraries(web-server mimosa-http)
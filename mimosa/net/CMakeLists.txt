install(FILES
  server.hh
  DESTINATION include/mimosa/net/)

add_library(mimosa-net
  connect.cc
  server.cc)
target_link_libraries(mimosa-net mimosa-runtime)

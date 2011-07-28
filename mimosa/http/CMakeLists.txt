add_library(mimosa-http
  dispatch-handler.cc
  status.cc)

target_link_libraries(mimosa-http mimosa-net)

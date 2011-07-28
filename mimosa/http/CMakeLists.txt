add_library(mimosa-http
  dispatch-handler.cc
  status.cc
  server.cc
  )

target_link_libraries(mimosa-http mimosa-stream mimosa-net)

add_subdirectory(gen)
add_subdirectory(samples)
add_library(mimosa-rpc
  basic-call.cc
  channel.cc
  server.cc
  service.cc
  service-map.cc)
target_link_libraries(mimosa-rpc mimosa-stream mimosa-net protobuf)

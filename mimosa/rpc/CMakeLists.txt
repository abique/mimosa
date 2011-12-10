install(FILES
  basic-call.hh
  call.hh
  channel.hh
  protocol.hh
  server.hh
  service.hh
  service-map.hh
  DESTINATION include/mimosa/rpc/)

add_subdirectory(gen)
add_subdirectory(samples)

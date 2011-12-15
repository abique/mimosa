install(FILES
  basic-call.hh
  call.hh
  channel.hh
  protocol.hh
  server.hh
  service.hh
  service-map.hh
  DESTINATION include/mimosa/rpc/)

add_library(mimosa-rpc
  basic-call.cc
  channel.cc
  server.cc
  service.cc
  service-map.cc)
target_link_libraries(mimosa-rpc mimosa)

install(TARGETS mimosa-rpc
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

add_subdirectory(gen)
add_subdirectory(samples)

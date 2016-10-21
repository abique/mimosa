install(FILES
  basic-call.hh
  bencode.hh
  call.hh
  channel.hh
  exception.hh
  http-call.hh
  json.hh
  log.hh
  protocol.hh
  server.hh
  service.hh
  service-map.hh
  DESTINATION include/mimosa/rpc/)

add_library(mimosa-rpc
  basic-call.cc
  channel.cc
  http-call.cc
  log.cc
  server.cc
  service.cc
  service-map.cc)
target_link_libraries(mimosa-rpc mimosa protobuf)

add_library(mimosa-rpc-bencode bencode.cc)
target_link_libraries(mimosa-rpc-bencode mimosa-rpc mimosa-bencode)

add_library(mimosa-rpc-json json.cc)
target_link_libraries(mimosa-rpc-json mimosa-rpc mimosa-json)

install(TARGETS mimosa-rpc mimosa-rpc-bencode mimosa-rpc-json
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

add_subdirectory(gen)
add_subdirectory(samples)

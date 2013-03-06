install(FILES
  basic-call.hh
  bencode.hh
  call.hh
  channel.hh
  http-service-handler.hh
  json.hh
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
target_link_libraries(mimosa-rpc mimosa protobuf)

add_library(mimosa-rpc-bencode bencode.cc)
target_link_libraries(mimosa-rpc-bencode mimosa-rpc mimosa-bencode)

add_library(mimosa-rpc-json json.cc)
target_link_libraries(mimosa-rpc-json mimosa-rpc mimosa-json)

add_library(mimosa-rpc-http http-service-handler.cc)
target_link_libraries(mimosa-rpc-http mimosa-rpc-json)

install(TARGETS mimosa-rpc mimosa-rpc-bencode mimosa-rpc-json mimosa-rpc-http
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

add_subdirectory(gen)
add_subdirectory(samples)

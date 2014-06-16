install(FILES
  chunked-stream.hh
  client-channel.hh
  coding.hh
  cookie.hh
  crud-handler.hh
  dispatch-handler.hh
  error-handler.hh
  fs-handler.hh
  handler.hh
  host-handler.hh
  method.hh
  method-handler.hh
  message-reader.hh
  message-reader.hxx
  message-writer.hh
  message-writer.hxx
  log.hh
  log-handler.hh
  redirect.hh
  request.hh
  request-reader.hh
  response.hh
  response-writer.hh
  server-channel.hh
  server.hh
  status.hh
  time.hh
  DESTINATION include/mimosa/http/)

mimosa_flex(request-lexer)
mimosa_bison(request-parser)

add_library(mimosa-http
  chunked-stream.cc
  client-channel.cc
  cookie.cc
  crud-handler.cc
  dispatch-handler.cc
  error-handler.cc
  fs-handler.cc
  host-handler.cc
  method.cc
  method-handler.cc
  mime-db.cc
  log.cc
  log-handler.cc
  redirect.cc
  request.cc
  request-reader.cc
  request-lexer.cc
  request-parser.cc
  request-writer.cc
  response.cc
  response-reader.cc
  response-writer.cc
  status.cc
  server.cc
  server-channel.cc
  time.cc
  )
target_link_libraries(mimosa-http
  mimosa-json
  mimosa-uri
  mimosa-fs
  mimosa)

install(TARGETS mimosa-http
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

add_subdirectory(tests)
add_subdirectory(samples)

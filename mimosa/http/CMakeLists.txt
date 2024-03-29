﻿install(FILES
  byte-range.hh
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
  log-handler.hh
  log.hh
  message-reader.hh
  message-reader.hxx
  message-writer.hh
  message-writer.hxx
  method-handler.hh
  method.hh
  mime-db.hh
  redirect.hh
  request.hh
  request-lexer.hh
  request-parser.hh
  request-reader.hh
  request-writer.hh
  response.hh
  response-reader.hh
  response-writer.hh
  server-channel.hh
  server.hh
  status.hh
  time.hh
  DESTINATION include/mimosa/http/)

mimosa_flex(request-lexer)
mimosa_bison(request-parser)

add_library(mimosa-http STATIC
  chunked-stream.cc
  chunked-stream.hh
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
target_link_libraries(mimosa-http PUBLIC
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

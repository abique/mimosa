install(FILES
  coding.hh
  cookie.hh
  dispatch-handler.hh
  error-handler.hh
  fs-handler.hh
  handler.hh
  method.hh
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
  cookie.cc
  dispatch-handler.cc
  error-handler.cc
  fs-handler.cc
  method.cc
  mime-db.cc
  log.cc
  log-handler.cc
  redirect.cc
  request.cc
  request-reader.cc
  request-lexer.cc
  request-parser.cc
  response.cc
  response-writer.cc
  status.cc
  server.cc
  server-channel.cc
  time.cc
  )
target_link_libraries(mimosa-http mimosa)

install(TARGETS mimosa-http
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

add_subdirectory(tests)
add_subdirectory(samples)
